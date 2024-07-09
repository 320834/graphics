#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  // constructor reads and builds the shader
  Shader();
  Shader(const char* vertexPath, const char* fragmentPath);
  Shader(Shader& shader); // copy constructor

  // the program ID
  unsigned int m_ID;
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

  unsigned int get_program();
};

inline Shader::Shader() {}

inline Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file’s buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(std::ifstream::failure e) {
    throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;
  // Vertex Shader
  {
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      const std::string compile_error = infoLog;
      throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + compile_error);
    };
  }

  // Fragment Shader
  {
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    int success;
    char infoLog[512];
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      const std::string compile_error = infoLog;
      throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + compile_error);
    };
  }

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertex);
  glAttachShader(m_ID, fragment);
  glLinkProgram(m_ID);

  int success;
  char infoLog[512];
  // print linking errors if any
  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

  if(!success)
  {
    glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
    const std::string link_error = infoLog;
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + link_error);
  }
  // delete shaders; they’re linked into our program and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

inline Shader::Shader(Shader& shader) {
  std::cout << "Copy constructor" << std::endl;
  shader.m_ID = m_ID;
}

inline void Shader::use() {
  glUseProgram(m_ID);
}

inline void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

inline void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

inline void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

inline unsigned int Shader::get_program() {
  return m_ID;
}

#endif //SHADER_H_
