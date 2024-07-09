#ifndef EXPERIMENET_ENGINE_H_
#define EXPERIMENET_ENGINE_H_

#include <string>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "experiment/cube.h"

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine {

public:
  Engine(
    const std::string window_name,
    const std::string vertex_shader,
    const std::string fragment_shader,
    const float width,
    const float height
  );
  ~Engine();

  Camera& camera();
  Shader& shader();
  GLFWwindow* glfw_window();

  void add_cube(const glm::vec3 position, const std::string texture = "");

  void loop(std::function<void(Engine&)> function);

  std::vector<Cube> m_cubes;
  const std::string m_window_name;
private:

  GLFWwindow* m_window;
  Shader m_shader;
  Camera m_camera;

  const float m_window_width;
  const float m_window_height;

  unsigned int m_VAO, m_VBO;

  glm::mat4 m_model;
  glm::mat4 m_view;
  glm::mat4 m_projection;

  unsigned int m_model_id;
  unsigned int m_projection_id;
  unsigned int m_view_id;

  float m_last_frame = 0.0f;
  float m_delta_time = 0.0f;

  void init_window();
  void init_vertex_buffers();
  void init_model_view_projection();
  void init_callbacks();
  void process_exit();
  void calculate_delta_time();

  void process_input_wasd();

  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

#endif //EXPERIMENET_ENGINE_H_
