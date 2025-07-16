#version 330 core
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D specular;    
  float shininess;
}; 

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform SpotLight spot_light;

vec3 spot_calc() {
  vec3 lightDir = normalize(spot_light.position - FragPos);
  float theta = dot(lightDir, normalize(-spot_light.direction));
  float epsilon = spot_light.cutOff - spot_light.outerCutoff;
  float intensity = clamp((theta - spot_light.outerCutoff) / epsilon, 0.0, 1.0);

  float dist = length(spot_light.position - FragPos);
  float acc = (
    1.0f / 
    ((spot_light.constant) + (dist * spot_light.linear) + (dist * dist * spot_light.quadratic) ));

  // if(theta > spot_light.cutOff) {

  vec3 ambient =
    spot_light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse =
    spot_light.diffuse * diff * 
    texture(material.diffuse, TexCoords).rgb;  
  
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular = spot_light.specular * spec * texture(material.specular, TexCoords).rgb;

  // accentuation based on distance
  ambient *= acc;
  diffuse *= acc;
  specular *= acc;

  // For smooth edges
  diffuse *= intensity;
  specular *= intensity;

  return ambient + diffuse + specular;
  // }

  // return spot_light.ambient * acc * texture(material.diffuse, TexCoords).rgb;
}

vec3 directional_calc() {
  vec3 lightDir = normalize(-directional_light.direction);

  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  
  // diffuse 
  vec3 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = directional_light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
  
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular = directional_light.specular * spec * texture(material.specular, TexCoords).rgb;
      
  return ambient + diffuse + specular;
}

vec3 point_calc() {
  float dist = length(point_light.position - FragPos);
  float acc = (1.0f / ((point_light.constant) + (dist * point_light.linear) + (dist * dist * point_light.quadratic) ));

  // Others
  // Ambient
  vec3 ambient = point_light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse 
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(point_light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse =
    point_light.diffuse * diff * 
    texture(material.diffuse, TexCoords).rgb;  
  
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 specular = texture(material.specular, TexCoords).rgb;

  return ambient * acc + diffuse * acc + specular * acc;
}

void main()
{
  // vec3 result = directional_calc();
  // vec3 result = point_calc();
  vec3 result = spot_calc();
  FragColor = vec4(result, 1.0);
}