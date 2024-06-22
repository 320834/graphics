#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WASD {
  bool w;
  bool a;
  bool s;
  bool d;
};

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

inline WASD processInputWASD(GLFWwindow *window) {
  const float cameraSpeed = 0.05f; // adjust accordingly

  bool w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
  bool a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  bool s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
  bool d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

  return {
    .w = w,
    .a = a,
    .s = s,
    .d = d
  };

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
