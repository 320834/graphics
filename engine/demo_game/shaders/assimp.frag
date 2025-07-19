#version 330 core
out vec4 FragColor;

#define MAX_POINT_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_diffuse2;
  sampler2D texture_diffuse3;
  sampler2D texture_specular1;
  sampler2D texture_specular2;
  float shininess;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform int point_lights_size;

vec3 point_calc(vec3 norm, vec3 viewDir, PointLight point_light) {
  float dist = length(point_light.position - FragPos);
  float acc = (1.0f / ((point_light.constant) + (dist * point_light.linear) + (dist * dist * point_light.quadratic) ));

  // Others
  // Ambient
  vec3 ambient =
    point_light.ambient *
    texture(material.texture_diffuse1, TexCoords).rgb;

  // diffuse 
  // vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(point_light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse =
    point_light.diffuse *
    diff * 
    texture(material.texture_diffuse1, TexCoords).rgb;  
  
  // specular
  // vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular =
    point_light.specular *
    spec *
    texture(material.texture_specular1, TexCoords).rgb;

  return ambient * acc + diffuse * acc + specular * acc;
}

vec3 directional_calc(vec3 norm, vec3 viewDir) {
  vec3 lightDir = normalize(-directional_light.direction);

  // ambient
  vec3 ambient =
    directional_light.ambient *
    texture(material.texture_diffuse1, TexCoords).rgb;
  
  // diffuse 
  // vec3 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse =
    directional_light.diffuse *
    diff *
    texture(material.texture_diffuse1, TexCoords).rgb;
  
  // specular
  // vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular =
    directional_light.specular *
    spec *
    texture(material.texture_specular1, TexCoords).rgb;
      
  return ambient + diffuse + specular;
}

void main()
{
  // vec3 norm = normalize(Normal);
  // vec3 viewDir = -normalize(viewPos - FragPos);

  // vec3 result = vec3(0.0f, 0.0f, 0.0f);
  // result += directional_calc(norm, viewDir);
  // for(
  //   int i = 0;
  //   i < point_lights_size && i < MAX_POINT_LIGHTS;
  //   ++i)
  // {
  //   result += point_calc(norm, viewDir, point_lights[i]);
  // }

  // FragColor = vec4(result, 1.0f);

  FragColor = texture(material.texture_diffuse1, TexCoords);
}