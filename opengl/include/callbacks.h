#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

inline void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

inline bool processInputLeft(GLFWwindow *window)
{  
  return glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
}

inline bool processInputRight(GLFWwindow *window)
{  
  return glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
}

inline bool processInputUp(GLFWwindow *window)
{  
  return glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
}

inline bool processInputDown(GLFWwindow *window)
{
  return glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
}

inline bool processInputW(GLFWwindow *window)
{
  return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
}

inline bool processInputS(GLFWwindow *window)
{
  return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
}

#endif //CALLBACKS_H_
