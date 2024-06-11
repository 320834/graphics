#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct InitReturn {
  int status;
  GLFWwindow* window;
};

inline InitReturn init(const std::string window_name) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(800, 600, window_name.c_str(), NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return {
      .status=-1,
      .window=NULL
    };
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return {
      .status=-1,
      .window=NULL
    };
  }

  const int viewport_width = 800;
  const int viewport_height = 600;
  glViewport(0, 0, viewport_width, viewport_height);

  return {
      .status = 0,
      .window = window
    };
}

inline void log_potential_error(
  const std::string& message,
  const unsigned int shader)
{

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << message <<
      infoLog << std::endl;
  }

}

#endif //UTILS_H_
