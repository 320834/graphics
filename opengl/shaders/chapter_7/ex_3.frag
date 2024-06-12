#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 textCoords;

uniform sampler2D ourTextureOne;
uniform sampler2D ourTextureTwo;

void main() {
  // Showcase mixing texture one and simple color
  FragColor = texture(ourTextureOne, textCoords) * vec4(ourColor, 1.0);
  
  FragColor = mix(
    texture(ourTextureOne, textCoords),
    texture(ourTextureTwo, vec2(textCoords.x, textCoords.y)),
    1.0 // Interpolated value. How much to show for second image.
  );
}