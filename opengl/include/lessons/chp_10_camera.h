#ifndef CHP_10_CAMERA_H_
#define CHP_10_CAMERA_H_

#include "image/stb_image.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

extern float vertices[];

extern glm::vec3 cubePositions[];

extern float g_lastX;
extern float g_lastY;
extern bool g_isFirst;
extern float g_yaw;
extern float g_pitch;
extern float g_zoom;

void load_textures(int widths[2], int heights[2], int nrChannels[2], unsigned int textures[2]);
void generate_buffers(unsigned int *VAO, unsigned int *VBO);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::mat4 custom_look_at(
  glm::vec3 camera_pos,
  glm::vec3 camera_target,
  glm::vec3 up
);

int chp_10_camera_look_at();
int chp_10_freeroam();

int chp_10_ex_1_and_2();

#endif //CHP_10_CAMERA_H_
