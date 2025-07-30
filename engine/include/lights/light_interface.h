#ifndef LIGHTS_LIGHT_INTERFACE_H_
#define LIGHTS_LIGHT_INTERFACE_H_

#include <glm/glm.hpp>

#include "shader.h"

class LightInterface {

public:

  virtual void set_light(Shader& shader) = 0;

  void set_ambient(const glm::vec3& ambient);
  void set_diffuse(const glm::vec3& diffuse);
  void set_specular(const glm::vec3& specular);

  glm::vec3 ambient();
  glm::vec3 diffuse();
  glm::vec3 specular();

protected:

  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
};

inline void LightInterface::set_ambient(const glm::vec3& ambient) {
  m_ambient = ambient;
}

inline void LightInterface::set_diffuse(const glm::vec3& diffuse) {
  m_diffuse = diffuse;
}

inline void LightInterface::set_specular(const glm::vec3& specular) {
  m_specular = specular;
}

inline glm::vec3 LightInterface::ambient() {
  return m_ambient;
}

inline glm::vec3 LightInterface::diffuse() {
  return m_diffuse;
}

inline glm::vec3 LightInterface::specular() {
  return m_specular;
}

#endif //LIGHTS_LIGHT_INTERFACE_H_
