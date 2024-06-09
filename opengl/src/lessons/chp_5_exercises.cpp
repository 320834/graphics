#include "lessons/chp_5_hello_triangle.h"
#include "utils.h"
#include "callbacks.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * Try to draw 2 triangles next to each other using
 * glDrawArrays by adding more 
 * vertices to your data
*/
int chp_5_exercise_1_1() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Chp 5 Ex 1.1", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const int viewport_width = 800;
  const int viewport_height = 600;
  glViewport(0, 0, viewport_width, viewport_height);

  // Do Setup here
  // Vertex Shader

  unsigned int vertexShader;
  {
    const char* vertexShaderSource = 
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", vertexShader);
  }

  // Get fragment shader
  unsigned int fragmentShader;
  {
    const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", fragmentShader);
  }

  // Link Shaders
  unsigned int shaderProgram;
  {
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  // setup vertices
  float vertices[] = {
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    -0.5f, -0.0f, 0.0f, // left
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    0.5f, -0.0f, 0.0f, // right
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  // For wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Loop Here
  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();

  return 0;
}

int chp_5_exercise_1_2() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Chp 5 Ex 1.2", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const int viewport_width = 800;
  const int viewport_height = 600;
  glViewport(0, 0, viewport_width, viewport_height);

  // Do Setup here
  // Vertex Shader

  unsigned int vertexShader;
  {
    const char* vertexShaderSource = 
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", vertexShader);
  }

  // Get fragment shader
  unsigned int fragmentShader;
  {
    const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", fragmentShader);
  }

  // Link Shaders
  unsigned int shaderProgram;
  {
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  // setup vertices
  float vertices_one[] = {
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    -0.5f, -0.0f, 0.0f, // left
  };

  float vertices_two[] = {
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    0.5f, -0.0f, 0.0f, // right
  };

  unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);

  glBindVertexArray(VAOs[0]); // Will bound to 
  glBindBuffer(GL_ARRAY_BUFFER, VAOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_one), vertices_one, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // This is where the calls stores info to VAO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  // Do for second vbo and vao

  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_two), vertices_two, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // This is where the calls stores info to VAO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  // For wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Loop Here
  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgram);

  glfwTerminate();

  return 0;
}

int chp_5_exercise_1_3() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Chp 5 Ex 1.3", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const int viewport_width = 800;
  const int viewport_height = 600;
  glViewport(0, 0, viewport_width, viewport_height);

  // Do Setup here
  // Vertex Shader

  unsigned int vertexShader;
  {
    const char* vertexShaderSource = 
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", vertexShader);
  }

  // Get fragment shader
  unsigned int fragmentShader;
  {
    const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", fragmentShader);
  }

  unsigned int fragmentShaderTwo;
  {
    const char *fragmentShaderSourceTwo =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";

    fragmentShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderTwo, 1, &fragmentShaderSourceTwo, NULL);
    glCompileShader(fragmentShaderTwo);

    log_potential_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", fragmentShaderTwo);
  }

  // Link Shaders
  unsigned int shaderProgramOne;
  {
    shaderProgramOne = glCreateProgram();

    glAttachShader(shaderProgramOne, vertexShader);
    glAttachShader(shaderProgramOne, fragmentShader);
    glLinkProgram(shaderProgramOne);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgramOne, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgramOne, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders no longer needed
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
  }

  // Link Shaders
  unsigned int shaderProgramTwo;
  {
    shaderProgramTwo = glCreateProgram();

    glAttachShader(shaderProgramTwo, vertexShader);
    glAttachShader(shaderProgramTwo, fragmentShaderTwo);
    glLinkProgram(shaderProgramTwo);

    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders no longer needed
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
  }

  // setup vertices
  float vertices_one[] = {
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    -0.5f, -0.0f, 0.0f, // left
  };

  float vertices_two[] = {
    0.0f, 0.0f, 0.0f, // center
    0.0f, 0.5f, 0.0f, // top
    0.5f, -0.0f, 0.0f, // right
  };

  unsigned int VAOs[2], VBOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);

  glBindVertexArray(VAOs[0]); // Bound to first VAO
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_one), vertices_one, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // This is where the calls stores info to VAO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  // Bound second VAOs
  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_two), vertices_two, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // This is where the calls stores info to VAO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  // For wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Loop Here
  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramOne);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgramTwo);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgramOne);
  glDeleteProgram(shaderProgramTwo);

  glfwTerminate();

  return 0;
}