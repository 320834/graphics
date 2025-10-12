#version 330 core
out vec4 FragColor;

in vec2 textCoords;
uniform sampler2D texture_one;
uniform bool use_texture = false;

uniform vec4 color = vec4(1.0, 0.5, 1.0, 1.0);

void main() {

  if(use_texture) {
    FragColor = texture(texture_one, textCoords);
    // FragColor.r = texture(texture_one, textCoords).r;
    // FragColor.g = 0.5f;
    // FragColor.b = 1.0f;
    // FragColor.a = 1.0f;
  } else {
    FragColor = color;
  }

}
