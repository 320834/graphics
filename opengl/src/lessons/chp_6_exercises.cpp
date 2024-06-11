#include "lessons/chp_6_shaders.h"
#include "utils.h"
#include "callbacks.h"
#include "shader.h"

#include <cmath>

int chp_6_exercise_1()
{
  InitReturn obj = init("Chapter 6: Exercise 1.1");
  if(obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = obj.window;

  Shader shader("../shaders/chapter_6/chp6_ex_1_1.vert", "../shaders/chapter_6/chp6_ex_1_1.frag");

  // Set up vertices
  float vertices[] = {
    // positions // colors
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
  };

  // Generate VAO to store vertices and reuse
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. then set our vertex attributes pointers
  // This is where VBOs and EBOs bind to VAOs
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while(!glfwWindowShouldClose(window))
  {
    // Process Input
    processInput(window);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;
}

int chp_6_exercise_2()
{
  InitReturn obj = init("Chapter 6: Exercise 1.2");
  if(obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = obj.window;

  Shader shader("../shaders/chapter_6/chp6_ex_1_2.vert", "../shaders/chaper_6/chp6_ex_1_2.frag");

  // Set up vertices
  float vertices[] = {
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f // top
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    float timeValue = glfwGetTime();
    float offset_value = (sin(timeValue) / 2.0f);
    shader.setFloat("offset", offset_value);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;
}

int chp_6_exercise_3()
{
  InitReturn obj = init("Chapter 6: Exercise 1.3");
  if(obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = obj.window;

  Shader shader("../shaders/chapter_6/chp6_ex_1_3.vert", "../shaders/chapter_6/chp6_ex_1_3.frag");

  // Set up vertices
  float vertices[] = {
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f // top
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    float offset_value = 0.0;
    shader.setFloat("offset", offset_value);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();  

  return 0;
}