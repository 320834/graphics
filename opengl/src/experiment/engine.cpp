#include "experiment/engine.h"

#include "shader.h"
#include "camera.h"
#include "experiment/cube.h"
#include "experiment/scene.h"
#include "utils.h"

#include <memory>

class SceneInterface;

void Engine::add_engine_instance(
  std::shared_ptr<Engine> engine
)
{
  if(!engine) {
    utils::log("Engine parameter not valid", "Engine (add_engine_instance): ");
    return;
  }

  if(m_engine) {
    utils::log("Engine member already set", "Engine (add_engine_instance): ");
    return;
  }

  m_engine = engine;
}

bool Engine::add_event(
  const std::string& scene_name,
  const std::string& event_name,
  const std::function<void(std::shared_ptr<Engine>)>& event_handler
)
{
  auto scene = m_scene_events.find(scene_name);
  
  if(scene != m_scene_events.end()) {
    // No scene found

    auto event_search = scene->second.find(event_name);
    if(event_search != scene->second.end()) {
      // Event with name already added
      utils::log(
        "Failed to add event: " + scene_name + " : " + event_name,
        "EventManager"
      );
      return false;
    }

    m_scene_events[scene_name][event_name] = event_handler;

    return true;
  }

  // Condition that scene has not been added
  std::unordered_map<std::string, std::function<void(std::shared_ptr<Engine>)>>
    scene_events;

  scene_events.insert({event_name, event_handler});
  m_scene_events.insert({scene_name, scene_events});

  return true;
}

bool Engine::invoke_event(
  const std::string& scene_name,
  const std::string& event_name
)
{
  if(
    auto scene_search = m_scene_events.find(scene_name);
    scene_search != m_scene_events.end()
  ) {
    std::unordered_map<
      std::string,
      std::function<void(std::shared_ptr<Engine>)>
    > events = scene_search->second;

    if(
      auto event_search = events.find(event_name);
      event_search != events.end()
    ) {
      std::function<void(std::shared_ptr<Engine>)> func = event_search->second;
      
      if(m_engine) {
        func(m_engine);
      } else {
        utils::log("Cannot invoke event. Engine instance null. Are you sure you set a shared_ptr to the engine after insantiating an engine", "Engine");
      }
    }
  }

  return false;
}

TextureStatus Engine::TextureManager::get_texture(
  const std::string& texture_name
)
{
  std::string a = texture_name;
  auto search = m_textures.find(a);
  if(search != m_textures.end()) {
    return {
      .texture_id = search->second,
      .success = true
    };
  }

  TextureStatus status =
    import_texture_file(texture_name);

  if(status.success) {

    utils::log(
      "Loading Texture " + texture_name + " from disk",
      "Texture Manager");
    // Saving texture in map
    m_textures.insert({texture_name, status.texture_id});
  }

  return status;
}

TextureStatus Engine::TextureManager::import_texture_file(
  const std::string& filename
)
{
  if(filename.empty()) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  const std::string path = "../textures/" + filename;
  int width, height, nr_channels;
  
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height,
    &nr_channels, 0);

  // null that means something bad happened
  if(!data) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  auto pos = filename.find(".");
  if(pos == std::string::npos) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  const std::string extension =
    filename.substr(pos);

  TextureLoadData texture_data = {
    .width = width,
    .height = height,
    .nr_channels = nr_channels,
    .data = data
  };

  return load_texture(texture_data, extension);

}

TextureStatus Engine::TextureManager::load_texture(
  TextureLoadData& texture_data,
  const std::string extension
)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if(auto search = extension.find(".png"); search != std::string::npos) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, texture_data.data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, texture_data.data);
  }

  glGenerateMipmap(GL_TEXTURE_2D);
  // unsigned int use_texture_id = glGetUniformLocation(texture, "use_texture");
  // glUniform1i(use_texture_id, (int)true);

  stbi_image_free(texture_data.data);

  return {
    .texture_id = texture,
    .success = true
  };
}

std::shared_ptr<SceneInterface> Engine::SceneManager::get_current_scene() { 
  return m_scenes.at(m_current_active_scene);
}

void Engine::SceneManager::set_current_scene(
  const std::string& scene_name
) {
  if(
    auto search = m_scenes.find(scene_name);
    search != m_scenes.end()
  ) {
    m_current_active_scene = scene_name;
  } else {
    utils::log(
      "Failed to set scene (not found): " + scene_name,
      "SceneManager"
    );
  }

  // Do nothing if it cannot set. Maybe I should
  // throw it
}

std::string Engine::SceneManager::get_current_scene_name()
{
  return m_current_active_scene;
}

bool Engine::SceneManager::add_scene(
  std::shared_ptr<SceneInterface> new_scene
)
{
  const std::string scene_name =
    new_scene->scene_name();

  bool has_inserted = false;
  if(
    auto search = m_scenes.find(scene_name);
    search == m_scenes.end() // If not found
  ) {
    m_scenes.insert({scene_name, new_scene});
    has_inserted = true;
  } else {
    utils::log("Scene Name Already Added: " + scene_name, "SceneManager");
  }

  if(m_scenes.size() == 1) {
    set_current_scene(scene_name);
  }

  return has_inserted;
}

void Engine::SceneManager::delete_scene(
  const std::string& scene_name
)
{
  if(scene_name == m_current_active_scene) {
    utils::log(
      "Failed To Delete Scene " + scene_name + ": Scene is active",
      "SceneManager"
    );

    return;
  }

  if(
    auto search = m_scenes.find(scene_name);
    search != m_scenes.end()
  ) {
    m_scenes.erase(scene_name);
  }
}

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

Engine::SceneManager& Engine::scene_manager() {
  return m_scene_manager;
}

Engine::TextureManager& Engine::texture_manager() {
  return m_texture_manager;
}

void Engine::loop() {
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

    scene_manager().get_current_scene()->controls();
    scene_manager().get_current_scene()->render();
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

std::shared_ptr<Engine> create_engine(
  const std::string& window_name,
  const std::string& vertex_shader,
  const std::string& fragment_shader,
  const float width,
  const float height

)
{
  std::shared_ptr<Engine> engine =
    std::make_shared<Engine>(
      window_name,
      vertex_shader,
      fragment_shader,
      width,
      height
    );

  engine->add_engine_instance(engine);

  return engine;
}
