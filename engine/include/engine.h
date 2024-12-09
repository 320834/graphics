#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene_manager.h"
#include "shader.h"
#include "camera.h"

#include "opengl_wrapper.h"

template<class OpenGLWrapper>
class Engine : public std::enable_shared_from_this<Engine<OpenGLWrapper>> {
public:
  Engine(
    const std::string& window_name,
    const std::string& vertex_shader,
    const std::string& fragment_shader,
    const float width,
    const float height
  );
  ~Engine();


  void loop();
  
  GLFWwindow* glfw_window();
  Camera& camera();
  SceneManager& scene_manager();
  std::shared_ptr<Engine> engine();
  static Shader& shader();

private:

  OpenGLWrapper opengl;
  void init_window();
  void init_model_view_projection();
  void process_exit();
  void calculate_delta_time();
  void process_input_wasd();

  void init_callbacks();
  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

  const std::string m_window_name;
  float m_window_width;
  float m_window_height;

  unsigned int m_model_id;
  unsigned int m_projection_id;
  unsigned int m_view_id;

  glm::mat4 m_model;
  glm::mat4 m_view;
  glm::mat4 m_projection;

  float m_last_frame = 0.0f;
  float m_delta_time = 0.0f;

  static Shader m_shader;
  SceneManager m_scene_manager;
  Camera m_camera;
  GLFWwindow* m_glfw_window;
};

template <class OpenGLWrapper>
Shader Engine<OpenGLWrapper>::m_shader;

template <class OpenGLWrapper>
Shader& Engine<OpenGLWrapper>::shader() {
  return Engine<OpenGLWrapper>::m_shader;
}

#endif // ENGINE_H_
