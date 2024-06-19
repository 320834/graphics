#include "lessons/chp_8_transform.h"

#include "callbacks.h"
#include "shader.h"
#include "utils.h"
#include "image/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int chp_8_ex_1() {
  InitReturn win_obj = init("Chapter 8: Exercise 2");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_8/ex_2.vert", "../shaders/chapter_8/ex_2.frag");

  int widths[2], heights[2], nrChannels[2];
  unsigned int textures[2];
  {
    unsigned char *data_one = stbi_load("../textures/container.jpg", &widths[0], &heights[0],
      &nrChannels[0], 0);

    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    if(data_one) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widths[0], heights[0], 0, GL_RGB,
        GL_UNSIGNED_BYTE, data_one);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture one" << std::endl;
    }

    stbi_image_free(data_one);
  }
  
  {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data_two = stbi_load("../textures/awesomeface.png", &widths[1], &heights[1],
      &nrChannels[1], 0);

    glGenTextures(1, &textures[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(data_two) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widths[1], heights[1], 0, GL_RGBA,
        GL_UNSIGNED_BYTE, data_two);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture two" << std::endl;
    }
    stbi_image_free(data_two);
  }

  float vertices[] = {
    // positions // colors // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
  };

  unsigned int indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  unsigned int VAO, VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans_one = glm::mat4(1.0f);
  // trans = glm::translate(trans, glm::vec3(0.1f, 0.1f, 0.0f));
  // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  trans_one = glm::scale(trans_one, glm::vec3(0.5, 0.5, 0.5));
  trans_one = glm::translate(trans_one, glm::vec3(1.0f, 0.4f, 0.0f));

  glm::mat4 trans_two = glm::mat4(1.0f);
  // trans = glm::translate(trans, glm::vec3(0.1f, 0.1f, 0.0f));
  // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  trans_two = glm::scale(trans_two, glm::vec3(0.5, 0.5, 0.5));
  trans_two = glm::translate(trans_two, glm::vec3(-1.0f, 0.4f, 0.0f));

  shader.use();

  unsigned int transformLoc = glGetUniformLocation(shader.get_program(), "transform");

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureOne"), 0);
  shader.setInt("ourTextureOne", 0);

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureTwo"), 0);
  shader.setInt("ourTextureTwo", 1);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(VAO);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans_one));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    float timeValue = glfwGetTime();
    float sin_value = (sin(timeValue) / 2.0f) + 1.0f;
    float scale_factor = 1.0f;
    if (sin_value > 1.0) {
      scale_factor = 1.01f;
    } else {
      scale_factor = 0.99f;
    }

    trans_two = glm::scale(trans_two, glm::vec3(scale_factor, scale_factor, 0.99));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans_two));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;
}