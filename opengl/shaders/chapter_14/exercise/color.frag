#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform vec3 lightPos;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Light light;
uniform Material material;

void main() {
  // ambient
  vec3 ambient = material.ambient * light.ambient;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightRay = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightRay), 0.0f);
  vec3 diffuse = material.diffuse * diff * light.diffuse;

  // specular
  float specularStrength = 0.2f;
  vec3 view_dir = normalize(viewPos - FragPos);
  vec3 reflect_dir = reflect(-lightRay, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = material.specular * spec * light.specular;

  vec3 result = (ambient + diffuse + specular) * objectColor;

  FragColor = vec4(result, 1.0);
}
