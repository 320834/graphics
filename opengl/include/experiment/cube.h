#ifndef EXPERIMENT_CUBE_H_
#define EXPERIMENT_CUBE_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

const float vertices_cube[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

class Cube {

public:
  Cube(const unsigned int shader_id);
  Cube(const unsigned int shader_id, const glm::vec3&& position);

  void Render();
  void SetPosition(const glm::vec3& position);
  void SetScale(const float scale);

  glm::vec3 GetPosition();

private:

  float m_scale = 1.0f;
  glm::vec3 m_position;
  glm::mat4 m_model;
  const unsigned m_shader_id;

};

inline Cube::Cube(const unsigned int shader_id)
  : m_shader_id(shader_id) {
  
  m_model = glm::mat4(1.0f);
}

inline Cube::Cube(const unsigned int shader_id, const glm::vec3&& position)
  : m_shader_id(shader_id)
{
  m_model = glm::mat4(1.0f);
  m_position = position;
}

inline void Cube::Render() {

  m_model = glm::mat4(1.0f);
  m_model = glm::translate(m_model, m_position);
  m_model = glm::scale(m_model, glm::vec3(m_scale));

  unsigned int model_id = glGetUniformLocation(m_shader_id, "model");

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

inline void Cube::SetPosition(const glm::vec3& position) {
  m_position = position;
}

inline void Cube::SetScale(const float scale) {
  m_scale = scale;
}

inline glm::vec3 Cube::GetPosition() {
  return m_position;
}

#endif //EXPERIMENT_CUBE_H_
