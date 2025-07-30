#include "lights/point.h"

namespace {

AttenuationConfig default_att_config = {
  .constant = 1.0f,
  .linear = 0.09f,
  .quadratic = 0.032f
};

} // namespace

PointLight::PointLight(
  const int index
) : PointLight(index, glm::vec3(0.0f, 0.0f, 0.0f))
{}

PointLight::PointLight(
  const int index,
  const glm::vec3& position
) : PointLight(index, position, default_att_config)
{}

PointLight::PointLight(
  const int index,
  const glm::vec3& position,
  const AttenuationConfig& config
) : m_index{index},
    m_position{position},
    m_constant{config.constant},
    m_linear{config.linear},
    m_quadratic{config.quadratic}
{}

void PointLight::set_light(Shader& shader) {
  const std::string light =
    "point_lights[" + std::to_string(m_index) + "]";

    shader.use();
    shader.setVec3(light + ".position", m_position);

    shader.setVec3(light + ".ambient", m_ambient);
    shader.setVec3(light + ".diffuse", m_diffuse);
    shader.setVec3(light + ".specular", m_specular);

    shader.setFloat(light + ".constant", m_constant);
    shader.setFloat(light + ".linear", m_linear);
    shader.setFloat(light + ".quadratic", m_quadratic);
}

void PointLight::set_position(
  const glm::vec3 position
)
{
  m_position = position;
}

glm::vec3 PointLight::position() {
  return m_position;
}