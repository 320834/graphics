#ifndef DIRECTIONAL_H_
#define DIRECTIONAL_H_

#include "lights/light_interface.h"
#include "shader.h"

class DirectionalLight : public LightInterface {

public:

  DirectionalLight();
  DirectionalLight(
    const glm::vec3& direction
  );

  void set_light(Shader& shader) override;

  void set_direction(const glm::vec3& dir);
  glm::vec3 direction();

private:
  glm::vec3 m_direction;
};

#endif //DIRECTIONAL_H_
