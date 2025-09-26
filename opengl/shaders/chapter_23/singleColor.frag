#version 330 core
out vec4 FragColor;

in vec2 textCoords;
uniform sampler2D ourTextureOne;

void main() {
  FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
