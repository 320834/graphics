#version 330 core
out vec4 FragColor;

// in vec3 ourColor;
in vec2 textCoords;

uniform sampler2D ourTextureOne;
uniform sampler2D ourTextureTwo;

void main() {
  // Showcase mixing texture one and simple color
  //  * vec4(ourColor, 1.0)
  FragColor = texture(ourTextureOne, textCoords) * vec4(1.0, 0.5, 1.0, 1.0);
  
  FragColor = mix(
    texture(ourTextureOne, textCoords),
    texture(ourTextureTwo, textCoords),
    0.35 // Interpolated value. How much to show for second image.
  );
}