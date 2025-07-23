#include "engine.h"
#include "opengl_wrapper.h"
#include "constants.h"

#include <glm/gtc/type_ptr.hpp>

// ======================================

template <class OpenGLWrapper>
Engine<OpenGLWrapper>::Engine(
    const std::string& window_name,
    const float width,
    const float height,
    const ShaderFiles& shaders
) : 
  m_window_name(window_name),
  m_window_width(width),
  m_window_height(height)
{
  init_window();
  Engine<OpenGLWrapper>::m_simple_shader = Shader(
    shaders.simple_vertex_shader.c_str(),
    shaders.simple_fragment_shader.c_str()
  );

  Engine<OpenGLWrapper>::m_assimp_shader = Shader(
    shaders.assimp_vertex_shader.c_str(),
    shaders.assimp_fragment_shader.c_str()
  );

  init_model_view_projection();
  init_callbacks();
}

template <class OpenGLWrapper>
Engine<OpenGLWrapper>::~Engine() {

  glDeleteProgram(m_simple_shader.get_program());
  glfwTerminate();
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::loop() {
  while(!glfwWindowShouldClose(m_glfw_window)) {
    process_exit();
    calculate_delta_time();
    process_input_wasd();

    // Need to read more on this
    glfwSwapBuffers(m_glfw_window);
    glfwPollEvents();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glBindVertexArray(m_VAO);
    m_view = m_camera.GetViewMatrix();
    m_projection = glm::perspective(glm::radians(m_camera.Zoom), m_window_width / m_window_height, 0.1f, 100.0f);

    m_simple_shader.use();
    m_simple_shader.setMat4("view", m_view);
    m_simple_shader.setMat4("projection", m_projection);

    m_assimp_shader.use();
    m_assimp_shader.setMat4("view", m_view);
    m_assimp_shader.setMat4("projection", m_projection);

    scene_manager().get_current_scene()->base_render();
    if(
        m_control_timer.elapsed_milliseconds() >
        constants::CONTROL_DELAY_MILLISECONDS
      ) {
      scene_manager().get_current_scene()->controls();
      m_control_timer.reset();
    }
  }
}

template <class OpenGLWrapper>
Camera& Engine<OpenGLWrapper>::camera() {
  return m_camera;
}

template <class OpenGLWrapper>
GLFWwindow* Engine<OpenGLWrapper>::glfw_window() {
  return m_glfw_window;
}

template <class OpenGLWrapper>
SceneManager& Engine<OpenGLWrapper>::scene_manager() {
  return m_scene_manager;
}

template <class OpenGLWrapper>
std::shared_ptr<Engine<OpenGLWrapper>> Engine<OpenGLWrapper>::engine() {
  return this->shared_from_this();
}

// ==============================
// Private

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::init_window() {

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

  m_glfw_window = window;

}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::init_model_view_projection() {

  m_view = m_camera.GetViewMatrix();
  m_projection = glm::perspective(glm::radians(m_camera.Zoom), m_window_width / m_window_height, 0.1f, 100.0f);
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::process_exit() {
  if(glfwGetKey(m_glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_glfw_window, true);
  }
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::calculate_delta_time() {
  float current_frame = glfwGetTime();

  m_delta_time = current_frame - m_last_frame;
  m_last_frame = current_frame;
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::process_input_wasd() {

  // if(!camera_move()) return;

  bool w = glfwGetKey(m_glfw_window, GLFW_KEY_W) == GLFW_PRESS;
  bool a = glfwGetKey(m_glfw_window, GLFW_KEY_A) == GLFW_PRESS;
  bool s = glfwGetKey(m_glfw_window, GLFW_KEY_S) == GLFW_PRESS;
  bool d = glfwGetKey(m_glfw_window, GLFW_KEY_D) == GLFW_PRESS;

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

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::init_callbacks() {

  glfwSetWindowUserPointer(m_glfw_window, this);

  glfwSetCursorPosCallback(m_glfw_window, &Engine::mouse_callback);
  glfwSetScrollCallback(m_glfw_window, &Engine::scroll_callback);
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  
  void* user_data = glfwGetWindowUserPointer(window);

  if(!user_data) return;

  Engine* engine = reinterpret_cast<Engine*>(user_data);

  if(!engine) return;

  // if(!engine->camera_pan()) return;

  engine->camera().ProcessMouseMovement(xpos, ypos);
}

template <class OpenGLWrapper>
void Engine<OpenGLWrapper>::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

  void* user_data = glfwGetWindowUserPointer(window);

  if(!user_data) return;

  Engine* engine = reinterpret_cast<Engine*>(user_data);

  if(!engine) return;

  // if(!engine->camera_pan()) return;

  engine->camera().ProcessMouseScroll(yoffset);
}

template class Engine<OpenGLWrapper>;
