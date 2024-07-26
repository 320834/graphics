#include "experiment/engine.h"

#include "shader.h"
#include "camera.h"
#include "experiment/cube.h"
#include "utils.h"

Engine::Engine(
  const std::string window_name,
  const std::string vertex_shader,
  const std::string fragment_shader,
  const float width,
  const float height
) : 
  m_window_name(window_name),
  m_window_width(width),
  m_window_height(height)
{

  init_window();

  Shader shader(
    vertex_shader.c_str(),
    fragment_shader.c_str()
  );

  m_shader = shader;
  m_shader.use();

  init_vertex_buffers();
  init_model_view_projection();
  init_callbacks();
}

Engine::~Engine() {

  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteProgram(m_shader.get_program());

  glfwTerminate();
}

Camera& Engine::camera() {
  return m_camera;
}

Shader& Engine::shader() {
  return m_shader;
}

GLFWwindow* Engine::glfw_window() {
  return m_window;
}

void Engine::add_cube(
  const glm::vec3 position, const std::string texture
)
{
  m_cubes.emplace_back(
    m_shader.get_program(),
    position,
    texture
  );
}

void Engine::loop(std::function<void(Engine&)> function) {
  while(!glfwWindowShouldClose(m_window)) {
    process_exit();
    calculate_delta_time();
    process_input_wasd();

    // Need to read more on this
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(m_VAO);
    m_view = m_camera.GetViewMatrix();
    m_projection = glm::perspective(glm::radians(m_camera.Zoom), m_window_width / m_window_height, 0.1f, 100.0f);

    glUniformMatrix4fv(m_view_id, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(m_projection_id, 1, GL_FALSE, glm::value_ptr(m_projection));
    // Do commands here

    function(*this);

    // for(Cube cube : m_cubes) {
    //   float time = glfwGetTime();
    //   float translate_factor = (sin(glfwGetTime()) / 1.0f);

    //   cube.SetPosition(glm::vec3(cube.GetPosition().x + translate_factor, cube.GetPosition().y, cube.GetPosition().z));
    //   cube.Render();
    // }
  }
}

void Engine::init_window() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window =
    glfwCreateWindow(m_window_width, m_window_height, m_window_name.c_str(), NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  #ifdef __APPLE__
  int width = (int)m_window_width;
  int height = (int)m_window_height;
  glfwGetFramebufferSize(window, &width, &height);

  m_window_width = static_cast<float>(width);
  m_window_height = static_cast<float>(height);
  #endif

  glViewport(0, 0, m_window_width, m_window_height);

  m_window = window;

}

void Engine::init_vertex_buffers() {

  glGenBuffers(1, &m_VBO);
  glGenVertexArrays(1, &m_VAO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube::vertices_cube), cube::vertices_cube, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cube::g_cube_stride * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, cube::g_cube_stride * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void Engine::init_model_view_projection() {
  m_model = glm::mat4(1.0f);
  m_view = m_camera.GetViewMatrix();
  m_projection = glm::perspective(glm::radians(m_camera.Zoom), m_window_width / m_window_height, 0.1f, 100.0f);

  m_model_id = glGetUniformLocation(m_shader.get_program(), "model");
  m_view_id = glGetUniformLocation(m_shader.get_program(), "view");
  m_projection_id = glGetUniformLocation(m_shader.get_program(), "projection");

  glUniformMatrix4fv(m_model_id, 1, GL_FALSE, glm::value_ptr(m_model));
  glUniformMatrix4fv(m_view_id, 1, GL_FALSE, glm::value_ptr(m_view));
  glUniformMatrix4fv(m_projection_id, 1, GL_FALSE, glm::value_ptr(m_projection));

}

void Engine::init_callbacks() {

  glfwSetWindowUserPointer(m_window, this);

  glfwSetCursorPosCallback(m_window, &Engine::mouse_callback);
  glfwSetScrollCallback(m_window, &Engine::scroll_callback);
}

void Engine::process_exit() {
  if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window, true);
  }
}

void Engine::calculate_delta_time() {
  float current_frame = glfwGetTime();

  m_delta_time = current_frame - m_last_frame;
  m_last_frame = current_frame;
}

void Engine::process_input_wasd() {
  bool w = glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS;
  bool a = glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS;
  bool s = glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS;
  bool d = glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS;

  if(a) {
    m_camera.ProcessKeyboard(Camera_Movement::LEFT, m_delta_time);
  } else if(d) {
    m_camera.ProcessKeyboard(Camera_Movement::RIGHT, m_delta_time);
  } else if(w) {
    m_camera.ProcessKeyboard(Camera_Movement::FORWARD, m_delta_time);
  } else if(s) {
    m_camera.ProcessKeyboard(Camera_Movement::BACKWARD, m_delta_time);
  }
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  void* user_data = glfwGetWindowUserPointer(window);

  if(!user_data) return;

  Engine* engine = reinterpret_cast<Engine*>(user_data);

  if(!engine) return;

  engine->camera().ProcessMouseMovement(xpos, ypos);
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  void* user_data = glfwGetWindowUserPointer(window);

  if(!user_data) return;

  Engine* engine = reinterpret_cast<Engine*>(user_data);

  if(!engine) return;

  engine->camera().ProcessMouseScroll(yoffset);
}
