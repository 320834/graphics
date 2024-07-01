#version 330 core
out vec4 FragColor;

in vec2 textCoords;
uniform sampler2D texture_one;
uniform bool use_texture = false;

void main() {

  if(use_texture) {
    FragColor = texture(texture_one, textCoords);
  } else {
    FragColor = vec4(1.0, 0.5, 1.0, 1.0);
  }

}