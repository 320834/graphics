#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture_manager.h"
#include "engine.h"

using VertexType = float;

class Shape {

public:
  Shape(const glm::vec3& position);
  Shape(
    const glm::mat4& transform,
    const glm::mat4& scale,
    const glm::mat4& rotation
  );

public:
  const glm::mat4 TransformMatrix() const;
  const glm::mat4 ScaleMatrix() const;
  const glm::mat4 RotateMatrix() const;

  void Scale(const float scale);
  void ScaleX(const float scale);
  void ScaleY(const float scale);
  void ScaleZ(const float scale);
  void Translate(const glm::vec3& delta);

  virtual void render() = 0;

protected:
  glm::mat4 m_transformation;
  glm::mat4 m_rotation;
  glm::mat4 m_scale;
};

inline Shape::Shape(const glm::vec3& position)
  : Shape(
    glm::translate(glm::mat4(1.0f), position),
    glm::mat4(1.0f),
    glm::mat4(1.0f)
  )
{}

inline Shape::Shape(
  const glm::mat4& transformation,
  const glm::mat4& scale,
  const glm::mat4& rotation
)
  : m_transformation{transformation},
    m_scale{scale},
    m_rotation{rotation}
{}

inline void Shape::Translate(const glm::vec3& delta) {
  m_transformation = glm::translate(m_transformation, delta);
}

inline void Shape::Scale(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(scale));
}

inline void Shape::ScaleX(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(scale, 1.0f, 1.0f));
}

inline void Shape::ScaleY(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(1.0f, scale, 1.0f));
}

inline void Shape::ScaleZ(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(1.0f, 1.0f, scale));
}

inline const glm::mat4 Shape::TransformMatrix() const {
  return m_transformation;
}

inline const glm::mat4 Shape::ScaleMatrix() const {
  return m_scale;
}

inline const glm::mat4 Shape::RotateMatrix() const {
  return m_rotation;
}

#endif //SHAPE_H_
