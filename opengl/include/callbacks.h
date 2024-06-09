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

#endif //CALLBACKS_H_
