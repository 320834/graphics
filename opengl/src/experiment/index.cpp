#include <iostream>
#include <vector>

#include "experiment/engine.h"

int runner() {

  Engine engine(
    "Engine Test", 
    "../shaders/experiment/simple.vert",
    "../shaders/experiment/simple.frag",
    1000,
    563
  );

  engine.add_cube(
    glm::vec3(1.0f, 1.0f, -5.0f),
    "container.jpg"
  );

  engine.add_cube(
    glm::vec3(0.0f, 0.0f, -10.0f),
    "container.jpg"
  );

  engine.add_cube(
    glm::vec3(-1.0f, 1.0f, -5.0f),
    "awesomeface.png"
  );

  engine.loop([](Engine& engine) {

    for(Cube& cube : engine.m_cubes) {

      if(glfwGetKey(engine.glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {
        cube.SetTexture("awesomeface.png");
      } else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        cube.SetTexture("container.jpg");
      } else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        cube.SetTexture("asef.df");
      }

      float time = glfwGetTime();
      float translate_factor = (sin(glfwGetTime()) / 1.0f);
      std::cout << translate_factor << std::endl;
      float d = 0.01;
      if(translate_factor <= 0) {
        d *= -1;
      }

      cube.Transform(glm::vec3(d, 0, 0));
      cube.Render();
    }
  });

  return 0;
}