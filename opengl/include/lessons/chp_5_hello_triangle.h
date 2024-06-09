#ifndef CHP_5_HELLO_TRIANGLE_H_
#define CHP_5_HELLO_TRIANGLE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Initialize {
  unsigned int shaderProgram;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int VBO;
};

int hello_triangle();
int chp_5_exercise_1_1();
int chp_5_exercise_1_2();
int chp_5_exercise_1_3();

#endif //CHP_5_HELLO_TRIANGLE_H_
