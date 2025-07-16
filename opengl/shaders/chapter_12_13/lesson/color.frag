#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

// uniform vec3 lightPos;

uniform vec3 lightsPos[3];

vec3 multipleLights(vec3 lightPos) {

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightRay = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightRay), 0.0f);
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.2f;
  vec3 view_dir = normalize(viewPos - FragPos);
  vec3 reflect_dir = reflect(-lightRay, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  // vec3 result = (ambient + diffuse + specular) * objectColor;
  return diffuse + specular;

}

void main() {

  // vec3 lightPos = lightsPos[1];

  // ambient
  float ambientStr = 0.1f;
  vec3 ambient = ambientStr * lightColor;

  vec3 diffuse_specular = vec3(0.0f, 0.0f, 0.0f);
  for(int i = 0; i < 1; ++i) {
    diffuse_specular +=
      multipleLights(lightsPos[i]);
  }

  vec3 result = (ambient + diffuse_specular) * objectColor;

  FragColor = vec4(result, 1.0);
}
