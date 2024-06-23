#include <iostream>
#include "lessons/chp_5_hello_triangle.h"
#include "lessons/chp_6_shaders.h"
#include "lessons/chp_7_textures.h"
#include "lessons/chp_8_transform.h"
#include "lessons/chp_9_coordinate_systems.h"
#include "lessons/chp_9_exercises.h"
#include "lessons/chp_10_camera.h"
#include "shader.h"

#include <glm/glm.hpp>

int main() {

  // hello_triangle();
  // chp_5_exercise_1_1();
  // chp_5_exercise_1_2();
  // chp_5_exercise_1_3();

  // chapter_6_shaders();
  // chp_6_exercise_1();
  // chp_6_exercise_2();
  // chp_6_exercise_3();

  // chp_7_textures();
  // chp_7_ex_1_and_4();
  // chp_7_ex_2();
  // chp_7_ex_3();

  // chp_8_transform();
  // chp_8_ex_1();

  // chp_9_coords();
  // chp_9_ex_3();

  // chp_10_camera_look_at();
  // chp_10_freeroam();
  // chp_10_ex_1_and_2();

  glm::mat4 translation {
    1.0f, 2.0f, 0.0f, -5,
    3.0f, 1.0f, 6.0f, -10,
    0.0f, 0.0f, 1.0f, -15,
    0.0f, 0.0f, 0.0f, 1
  };

  std::cout << translation[1][2] << std::endl;


  return 0;
}