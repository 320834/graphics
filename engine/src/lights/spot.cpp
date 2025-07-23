#include "lights/spot.h"

SpotLight::SpotLight(
  const int index
) : SpotLight(index, glm::vec3(0.0f, 0.0f, 0.0f))
{}

SpotLight::SpotLight(
  const int index,
  const glm::vec3& position
) :
  SpotLight(
    index, position,
    {
      .direction = glm::vec3(0.0f, 0.0f, -1.0f),
      .cutoff = glm::cos(glm::radians(4.5f)),
      .outer_cutoff = glm::cos(glm::radians(6.5f)),
      .attenuation = {
        .constant = 1.0f,
        .linear = 0.045f,
        .quadratic = 0.0075f
      }
    }
  )
{}

SpotLight::SpotLight(
  const int index,
  const glm::vec3& position,
  const SpotLightConfigs& config
) :
  m_index{index},
  m_position{position},
  m_direction{config.direction},
  m_cutoff{config.cutoff},
  m_outer_cutoff{config.outer_cutoff},
  m_constant{config.attenuation.constant},
  m_linear{config.attenuation.linear},
  m_quadratic{config.attenuation.quadratic}
{}

void SpotLight::set_light(Shader& shader) {
  const std::string light =
  "spot_lights[" + std::to_string(m_index) + "]";

  shader.use();
  shader.setVec3(light + ".position", m_position);
  shader.setVec3(light + ".direction", m_direction);

  shader.setFloat(light + ".cutOff", m_cutoff);
  shader.setFloat(light + ".outerCutoff", m_outer_cutoff);

  shader.setVec3(light + ".ambient", m_ambient);
  shader.setVec3(light + ".diffuse", m_diffuse);
  shader.setVec3(light + ".specular", m_specular);

  shader.setFloat(light + ".constant", m_constant);
  shader.setFloat(light + ".linear", m_linear);
  shader.setFloat(light + ".quadratic", m_quadratic);
}

void SpotLight::set_position(const glm::vec3 position) {
  m_position = position;
}

glm::vec3 SpotLight::position() {
  return m_position;
}

void SpotLight::set_direction(const glm::vec3 dir) {
  m_direction = dir;
}

glm::vec3 SpotLight::direction() {
  return m_direction;
}

void SpotLight::set_cutoffs(
  const float cutoff_degrees, const float outer_cutoff_degrees
)
{
  m_cutoff = glm::cos(glm::radians(cutoff_degrees));
  m_outer_cutoff = glm::cos(glm::radians(outer_cutoff_degrees));
}

std::pair<float, float> SpotLight::cutoffs() {
  return std::make_pair(m_cutoff, m_outer_cutoff);
}