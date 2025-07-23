#ifndef LIGHTS_SPOT_H_
#define LIGHTS_SPOT_H_

#include <utility>

#include "lights/light_interface.h"
#include "lights/point.h"

struct SpotLightConfigs {
  glm::vec3 direction;
  float cutoff;
  float outer_cutoff;
  AttenuationConfig attenuation;
};

class SpotLight : public LightInterface {
public:

  SpotLight(const int index);
  SpotLight(
    const int index,
    const glm::vec3& position
  );

  SpotLight(
    const int index,
    const glm::vec3& position,
    const SpotLightConfigs& config
  );

  void set_light(Shader& shader) override;

  void set_position(const glm::vec3 position);
  glm::vec3 position();

  void set_direction(const glm::vec3 dir);
  glm::vec3 direction();

  void set_cutoffs(const float cutoff_degrees, const float outer_cutoff_degrees);
  std::pair<float, float> cutoffs();

private:
  int m_index;
  glm::vec3 m_position;
  glm::vec3 m_direction;

  float m_cutoff;
  float m_outer_cutoff;

  float m_constant;
  float m_linear;
  float m_quadratic;

};

#endif //LIGHTS_SPOT_H_
