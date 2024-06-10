#version 330 core
layout (location = 0) in vec3 pos;

uniform float offset;
out vec3 color;

void main()
{
  gl_Position = vec4(pos.x + offset, pos.yz, 1.0);
  // color = vec3(pos.x + offset, pos.yz);
  color = pos;
}