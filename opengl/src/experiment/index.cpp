#include <iostream>
#include <vector>

#include "utils.h"
#include "shader.h"
#include "callbacks.h"
#include "camera.h"
#include "experiment/cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Camera camera;

void mouse_callback_expri(GLFWwindow* window, double xpos, double ypos) {
  camera.ProcessMouseMovement(xpos, ypos);

  // Some sort of ray tracer
  // Get Camera front
  // Shoot in x distance
  // and then use cube formula to see if intersect
}

void scroll_callback_expri(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

void process_keypress(const WASD& wasd, const float delta_time) {
  if(wasd.a) {
    camera.ProcessKeyboard(Camera_Movement::LEFT, delta_time);
  } else if(wasd.d) {
    camera.ProcessKeyboard(Camera_Movement::RIGHT, delta_time);
  } else if(wasd.w) {
    camera.ProcessKeyboard(Camera_Movement::FORWARD, delta_time);
  } else if(wasd.s) {
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, delta_time);
  }
}

int runner() {

  InitReturn win_obj = init("Experiment");
  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;
  Shader shader("../shaders/experiment/simple.vert", "../shaders/experiment/simple.frag");

  unsigned int VAO, VBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  view = camera.GetViewMatrix();
  projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int model_id = glGetUniformLocation(shader.get_program(), "model");
  unsigned int view_id = glGetUniformLocation(shader.get_program(), "view");
  unsigned int projection_id = glGetUniformLocation(shader.get_program(), "projection");

  shader.use();

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

  // TODO: Turn it into member variable for class
  float cameraSpeed = 0.05f;
  float deltaTime = 0.0f; // Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame

  glfwSetCursorPosCallback(window, mouse_callback_expri);
  glfwSetScrollCallback(window, scroll_callback_expri);

  std::vector<Cube> cubes;
  cubes.emplace_back(
    shader.get_program(),
    glm::vec3(1.0f, 1.0f, -5.0f)
  );
  cubes.emplace_back(
    shader.get_program(),
    glm::vec3(0.0f, 0.0f, -5.0f)
  );
  cubes.emplace_back(
    shader.get_program(),
    glm::vec3(-1.0f, 1.0f, -5.0f),
    "awesomeface.png"
  );

  while(!glfwWindowShouldClose(window)) {

    processInput(window);

    // TODO: Turn it into a function if put in a class
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    auto input = processInputWASD(window);
    process_keypress(input, deltaTime);
    const bool right = processInputRight(window);
    const bool left = processInputLeft(window);
    const bool up = processInputUp(window);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // std::cout << glm::to_string(camera.Front) << std::endl;

    // Enable depth test for z buffer
    // Render z values first
    glEnable(GL_DEPTH_TEST);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(Cube& cube : cubes) {
      if(right) {
        cube.SetTexture("awesomeface.png");
      } else if(up) {
        cube.SetTexture("asdf");
      } else if(left) {
        cube.SetTexture("container.jpg");
      }
    }

    // if(right) {
    //   for(Cube& cube : cubes) {
    //     cube.SetTexture("awesomeface.png");
    //   }
    // } else if(left) {
    //   for(Cube& cube : cubes) {
    //     cube.SetTexture("container.jpg");
    //   }
    // }

    glBindVertexArray(VAO);
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

    for(Cube cube : cubes) {
      float time = glfwGetTime();
      float translate_factor = (sin(glfwGetTime()) / 1.0f);

      cube.SetPosition(glm::vec3(cube.GetPosition().x + translate_factor, cube.GetPosition().y, cube.GetPosition().z));

      cube.Render();
    }
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;
}