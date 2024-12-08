#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

using VertexType = float;

class Shape {

public:
  Shape(const glm::vec3& position);
  Shape(
    const glm::mat4& transform,
    const glm::mat4& scale,
    const glm::mat4& rotation
  );

  const glm::mat4 TransformMatrix() const;
  const glm::mat4 ScaleMatrix() const;
  const glm::mat4 RotateMatrix() const;

  virtual void render() = 0;

protected:
  glm::mat4 m_transformation;
  glm::mat4 m_rotation;
  glm::mat4 m_scale;
};

inline Shape::Shape(const glm::vec3& position)
  : m_transformation{glm::translate(glm::mat4(1.0f), position)},
    m_rotation{glm::mat4(1.0f)},
    m_scale{glm::mat4(1.0f)}
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
