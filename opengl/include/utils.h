#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
