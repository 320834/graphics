#version 330 core
out vec4 FragColor;

in vec3 InFragColor;

void main() {
  FragColor = vec4(InFragColor, 1.0);
}
