#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoords;

out vec3 ourColor;
out vec2 textCoords;

void main() {
  gl_Position = vec4(pos, 1.0);

  ourColor = aColor;
  textCoords = aTextCoords;
}