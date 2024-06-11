#include "lessons/chp_7_textures.h"
#include "callbacks.h"
#include "shader.h"
#include "utils.h"
#include "image/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int chp_7_generate_texture();

int chp_7_textures()
{
  chp_7_generate_texture();
  return 0;
}

int chp_7_generate_texture() {

  InitReturn win_obj = init("Chapter 7: Generate Textures");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_7/generate_texture.vert", "../shaders/chapter_7/generate_texture.frag");

  int width, height, nrChannels;
  unsigned int texture;
  {
    unsigned char *data = stbi_load("../textures/container.jpg", &width, &height,
      &nrChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
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
    1, 2, 3  // second triangle
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

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
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