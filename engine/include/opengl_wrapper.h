#ifndef OPENGL_WRAPPER_H_
#define OPENGL_WRAPPER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
  Wrapper for opengl calls. This construct is for
  unit testing
*/

class OpenGLWrapper {

public:
  OpenGLWrapper() = default;
  ~OpenGLWrapper() = default;

  void glfwInit();
  void glfwWindowHint(int hint, int value);
  GLFWwindow* glfwCreateWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share);
  void glfwTerminate();
  void glfwMakeContextCurrent(GLFWwindow *window);
  void glfwSetInputMode(GLFWwindow *window, int mode, int value);
  void glfwGetFramebufferSize (GLFWwindow *window, int *width, int *height);
  void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

  // Glad functions
  int gladLoadGLLoader(GLADloadproc load);
};

inline void OpenGLWrapper::glfwInit() {
  glfwInit();
}

inline void OpenGLWrapper::glfwWindowHint(int hint, int value) {
  glfwWindowHint(hint, value);
}

inline GLFWwindow* OpenGLWrapper::glfwCreateWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share) {
  return glfwCreateWindow(width, height, title, monitor, share);
}

inline void OpenGLWrapper::glfwTerminate() {
  glfwTerminate();
}

inline void OpenGLWrapper::glfwMakeContextCurrent(GLFWwindow *window) {
  glfwMakeContextCurrent(window);
}

inline void OpenGLWrapper::
  glfwSetInputMode(GLFWwindow *window, int mode, int value) 
{
  glfwSetInputMode(window, mode, value);
}

inline void OpenGLWrapper::glfwGetFramebufferSize(GLFWwindow *window, int *width, int *height) {
  glfwGetFramebufferSize(window, width, height);
}

inline void OpenGLWrapper::glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  glViewport(x, y, width, height);
}

// ================
// Glad functions

inline int OpenGLWrapper::gladLoadGLLoader(GLADloadproc load) {
  return gladLoadGLLoader(load);
}

#endif // OPENGL_WRAPPER_H_