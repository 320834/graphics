#include <iostream>
#include <vector>

#include "experiment/engine.h"
#include "experiment/utils.h"

int runner() {

  Engine engine(
    "Engine Test", 
    "../shaders/experiment/simple.vert",
    "../shaders/experiment/simple.frag",
    1000,
    563
  );

  engine.add_cube(
    glm::vec3(0.0f, 0.0f, -5.0f),
    "awesomeface.png"
  );

  engine.add_cube(
    glm::vec3(1.0f, 0.0f, -5.0f),
    "container.jpg"
  );


  engine.loop([](Engine& engine) {

    auto col = engine.m_cubes[1].IsColliding(engine.m_cubes[0]);
    if(col.collide) {
      std::cout << "Is Colliding. Points: " << std::to_string(col.points) << std::endl;
    } else {
      std::cout << "Not Colliding" << std::endl;
    }

    for(Cube& cube : engine.m_cubes) {

      float time = glfwGetTime();
      float translate_factor = (sin(glfwGetTime()) / 1.0f);
      float d = 0.01;
      if(translate_factor <= 0) {
        d *= -1;
      }

      cube.Transform(glm::vec3(d, 0, 0));

      if(glfwGetKey(engine.glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {
        cube.ScaleY(1.05f);
      } 
      else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        cube.ScaleY(0.95);

      } 
      else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        cube.ScaleX(1.05);
      }
      else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cube.ScaleX(0.95);
      }

      cube.Render();
    }
  });

  return 0;
}
