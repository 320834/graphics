#include "lessons/chp_7_textures.h"
#include "utils.h"
#include "callbacks.h"
#include "shader.h"

#include "image/stb_image.h"

// Make sure only the happy face looks in the other/reverse 
// direction by changing the fragment shader
int chp_7_ex_1_and_4() {
  InitReturn win_obj = init("Chapter 7: Exercise 1");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_7/ex_1.vert", "../shaders/chapter_7/ex_1.frag");

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

  shader.use();

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureOne"), 0);
  shader.setInt("ourTextureOne", 0);

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureTwo"), 0);
  shader.setInt("ourTextureTwo", 1);

  float interpolated_val = 0.5;

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    if(processInputDown(window)) {
      interpolated_val -= 0.01;
    }

    if(processInputUp(window)) {
      interpolated_val += 0.01;
    }

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    shader.setFloat("interpolated", interpolated_val);

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
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

// Experiment with the different texture
// wrapping methods by specifying texture coordinates in
// the range 0.0f to 2.0f instead of 0.0f to 1.0f.
// See if you can display 4 smiley faces on a
// single container image clamped at its edge

int chp_7_ex_2() {
  InitReturn win_obj = init("Chapter 7: Exercise 2");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_7/ex_2.vert", "../shaders/chapter_7/ex_2.frag");

  int widths[2], heights[2], nrChannels[2];
  unsigned int textures[2];
  {
    unsigned char *data_one = stbi_load("../textures/container.jpg", &widths[0], &heights[0],
      &nrChannels[0], 0);

    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    // positions      // colors         // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f // top left
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

  shader.use();

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

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
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

// Try to display only the center pixels of the 
// texture image on the rectangle in such a way
// that the individual pixels are getting visible
// by changing the texture coordinates. Try to
// set the texture filtering method to GL_NEAREST
// to see the pixels more clearly
int chp_7_ex_3() {
    InitReturn win_obj = init("Chapter 7: Exercise 3");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_7/ex_3.vert", "../shaders/chapter_7/ex_3.frag");

  int widths[2], heights[2], nrChannels[2];
  unsigned int textures[2];
  {
    unsigned char *data_one = stbi_load("../textures/container.jpg", &widths[0], &heights[0],
      &nrChannels[0], 0);

    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    // positions      // colors         // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 
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

  shader.use();

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

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
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