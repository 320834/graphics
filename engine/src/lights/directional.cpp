#include "lights/directional.h"

#include "shader.h"

DirectionalLight::DirectionalLight()
  : m_direction(glm::vec3(0.0f, 0.0f, -1.0f))
{}

DirectionalLight::DirectionalLight(
  const glm::vec3& direction
)
  : LightInterface(),  
    m_direction(direction)
{}

void DirectionalLight::set_light(
  Shader& shader
)
{
  shader.use();

  shader.setVec3("directional_light.direction", m_direction);

  shader.setVec3("directional_light.ambient", m_ambient);
  shader.setVec3("directional_light.diffuse", m_diffuse);
  shader.setVec3("directional_light.specular", m_specular);

}

void DirectionalLight::set_direction(
  const glm::vec3& dir
)
{
  m_direction = dir;
}

glm::vec3 DirectionalLight::direction() {
  return m_direction;
}