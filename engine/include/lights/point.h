#ifndef LIGHTS_POINT_H_
#define LIGHTS_POINT_H_

#include "lights/light_interface.h"

struct AttenuationConfig {
  float constant;
  float linear;
  float quadratic;
};

class PointLight : public LightInterface {
public:

  PointLight(const int index);

  PointLight(
    const int index,
    const glm::vec3& position
  );

  PointLight(
    const int index,
    const glm::vec3& position,
    const AttenuationConfig& config
  );

  void set_light(Shader& shader) override;

  void set_position(const glm::vec3 position);
  glm::vec3 position();

private:

  int m_index;
  glm::vec3 m_position;
  float m_constant;
  float m_linear;
  float m_quadratic;

};

#endif //LIGHTS_POINT_H_
