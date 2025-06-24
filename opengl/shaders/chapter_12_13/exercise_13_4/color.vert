#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 lightPos;

out vec3 InFragColor;
// out vec3 Normal;
// out vec3 FragPos;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0);
  vec3 FragPos = vec3(model * vec4(pos, 1.0));

  vec3 Normal = mat3(transpose(inverse(model))) * normal;

  // ambient
  float ambientStr = 0.1f;
  vec3 ambient = ambientStr * lightColor;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightRay = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightRay), 0.0f);
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.5;
  vec3 view_dir = normalize(viewPos - FragPos);
  vec3 reflect_dir = reflect(-lightRay, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  InFragColor = (ambient + diffuse + specular) * objectColor;



}
