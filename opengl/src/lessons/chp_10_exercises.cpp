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

glm::mat4 custom_look_at(
  glm::vec3 camera_pos,
  glm::vec3 camera_target,
  glm::vec3 up
)
{
  glm::vec3 camera_dir = glm::normalize(camera_pos - camera_target);
  glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_dir));
  glm::vec3 camera_up = glm::cross(camera_dir, camera_right);
  // top is right vector
  // middle is up vector
  // bott is direction vector
  glm::mat4 rotation {
    camera_right.x, camera_right.y , camera_right.z, 0.0f,
    camera_up.x   , camera_up.y    , camera_up.z   , 0.0f,
    camera_dir.x  , camera_dir.y   , camera_dir.z  , 0.0f,
    0.0f          , 0.0f           , 0.0f          , 1.0f
  };

  rotation = glm::transpose(rotation);

  glm::mat4 translation {
    1.0f, 0.0f, 0.0f, -camera_pos.x,
    0.0f, 1.0f, 0.0f, -camera_pos.y,
    0.0f, 0.0f, 1.0f, -camera_pos.z,
    0.0f, 0.0f, 0.0f, 1
  };

  translation = glm::transpose(translation);

  return rotation * translation;
}

int chp_10_ex_1_and_2()
{
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  InitReturn win_obj = init("Chapter 10: Exercise One");

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
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(VAO);

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    cameraFront = glm::normalize(direction);

    // EXERCISE 1: SOLUTION
    glm::vec3 move_front(cameraFront.x, 0.0f, cameraFront.z);

    if(input.w) {
      cameraPos += cameraSpeed * move_front;
    } else if(input.s) {
      cameraPos -= cameraSpeed * move_front;
    } else if(input.a) {
      cameraPos -= glm::normalize(glm::cross(move_front, cameraUp)) *
        cameraSpeed;
    } else if(input.d) {
      cameraPos += glm::normalize(glm::cross(move_front, cameraUp)) *
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
      // view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      view = custom_look_at(cameraPos, cameraPos + cameraFront, cameraUp);

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

