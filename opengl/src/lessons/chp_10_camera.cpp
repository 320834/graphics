#include "lessons/chp_10_camera.h"

#include "utils.h"
#include "callbacks.h"
#include "shader.h"
#include "image/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f, 0.0f, 0.0f),
  glm::vec3( 2.0f, 5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f, 3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f, 2.0f, -2.5f),
  glm::vec3( 1.5f, 0.2f, -1.5f),
  glm::vec3(-1.3f, 1.0f, -1.5f)
};

float g_lastX = 400;
float g_lastY = 300;
bool g_isFirst = true;
float g_yaw = -90.0f;
float g_pitch = 0.0f;
float g_zoom = 45.0f;

void load_textures(int widths[2], int heights[2], int nrChannels[2], unsigned int textures[2]) {
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

  std::cout << textures[0] << std::endl;
  std::cout << textures[1] << std::endl;
}

void generate_buffers(unsigned int *VAO, unsigned int *VBO)
{
  // unsigned int EBO;
  glGenBuffers(1, VBO);
  glGenVertexArrays(1, VAO);
  // glGenBuffers(1, &EBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

  if (g_isFirst) // initially set to true
  {
    g_lastX = xpos;
    g_lastY = ypos;
    g_isFirst = false;
  }

  float xoffset = xpos - g_lastX;
  float yoffset = g_lastY - ypos;

  g_lastX = xpos;
  g_lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  g_yaw += xoffset;
  g_pitch += yoffset;

  if(g_pitch > 89.0f)
    g_pitch = 89.0f;
  if(g_pitch < -89.0f)
    g_pitch = -89.0f;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  g_zoom -= (float)yoffset;
  if (g_zoom < 1.0f)
    g_zoom = 1.0f;
  if (g_zoom > 45.0f)
    g_zoom = 45.0f;
}

int chp_10_freeroam()
{
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  InitReturn win_obj = init("Chapter 10: Camera Look At");

  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_10/look_at.vert", "../shaders/chapter_10/look_at.frag");

  int widths[2], heights[2], nrChannels[2];
  unsigned int textures[2];
  load_textures(widths, heights, nrChannels, textures);

  unsigned int VAO, VBO;
  generate_buffers(&VAO, &VBO);


  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  projection = glm::perspective(glm::radians(g_zoom), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int model_id = glGetUniformLocation(shader.get_program(), "model");
  unsigned int view_id = glGetUniformLocation(shader.get_program(), "view");
  unsigned int projection_id = glGetUniformLocation(shader.get_program(), "projection");

  shader.use();

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureOne"), 0);
  shader.setInt("ourTextureOne", 0);

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureTwo"), 0);
  shader.setInt("ourTextureTwo", 1);

  float cameraSpeed = 0.05f;
  float deltaTime = 0.0f; // Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame

  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    auto input = processInputWASD(window);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cameraSpeed = 2.5f * deltaTime;


    // Enable depth test for z buffer
    // Render z values first
    glEnable(GL_DEPTH_TEST);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    std::cout << GL_TEXTURE0 << " " << textures[0] << std::endl;
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    std::cout << GL_TEXTURE0 << " " << textures[0] << std::endl;
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(VAO);

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    cameraFront = glm::normalize(direction);

    if(input.w) {
      cameraPos += cameraSpeed * cameraFront;
    } else if(input.s) {
      cameraPos -= cameraSpeed * cameraFront;
    } else if(input.a) {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    } else if(input.d) {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    }

    for(unsigned int i = 0; i < 10; i++) {
      model = glm::mat4(1.0f);
      view = glm::mat4(1.0f);

      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      // model = glm::rotate(model, glm::radians(angle),
      //   glm::vec3(1.0f, 0.3f, 0.5f));

      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f + 4 * i),
        glm::vec3(0.5f, 0.2f, 0.3f));
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

      projection = glm::perspective(glm::radians(g_zoom), 800.0f / 600.0f, 0.1f, 100.0f);

      glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;

}

int chp_10_camera_look_at()
{
InitReturn win_obj = init("Chapter 10: Camera Look At");

  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/chapter_10/look_at.vert", "../shaders/chapter_10/look_at.frag");

  int widths[2], heights[2], nrChannels[2];
  unsigned int textures[2];
  load_textures(widths, heights, nrChannels, textures);

  unsigned int VAO, VBO;
  generate_buffers(&VAO, &VBO);

  float fov = 45.0f;
  float camera_x = 0.0f;
  float camera_y = 0.0f;
  float camera_z = -3.0f;
  float y_up = 1.0f;

  // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(camera_x, camera_y, camera_z));
  projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int model_id = glGetUniformLocation(shader.get_program(), "model");
  unsigned int view_id = glGetUniformLocation(shader.get_program(), "view");
  unsigned int projection_id = glGetUniformLocation(shader.get_program(), "projection");

  shader.use();

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureOne"), 0);
  shader.setInt("ourTextureOne", 0);

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureTwo"), 0);
  shader.setInt("ourTextureTwo", 1);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // Enable depth test for z buffer
    // Render z values first
    glEnable(GL_DEPTH_TEST);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(VAO);

    if(processInputUp(window)) {
      if(y_up <= 1.0f) {
        y_up += 0.05f;
      }
    }

    if(processInputDown(window)) {
      if(y_up >= 0.0f) {
        y_up -= 0.05f;
      }
    }


    for(unsigned int i = 0; i < 10; i++) {
      model = glm::mat4(1.0f);
      view = glm::mat4(1.0f);

      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      // model = glm::rotate(model, glm::radians(angle),
      //   glm::vec3(1.0f, 0.3f, 0.5f));

      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f + 4 * i),
        glm::vec3(0.5f, 0.2f, 0.3f));

      const float radius = 10.0f;
      float camX = sin(glfwGetTime()) * radius;
      float camZ = cos(glfwGetTime()) * radius;

      // view = glm::translate(view, glm::vec3(camera_x, camera_y, camera_z));
      view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(1 - y_up, y_up, 0.0));

      projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

      glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;
}