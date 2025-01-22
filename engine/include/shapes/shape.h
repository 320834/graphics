#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture_manager.h"
#include "engine.h"

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

inline const Color DEFAULT_COLOR = {
  .r = 255,
  .g = 255,
  .b = 255
};

using VertexType = float;

class Shape {

public:
  Shape(const glm::vec3& position);
  Shape(
    const glm::vec3& position,
    const Color color
  );
  Shape(
    const glm::vec3& position,
    const std::string& texture_name
  );
  Shape(
    const glm::mat4& transform,
    const glm::mat4& scale,
    const glm::mat4& rotation,
    const int texture_id,
    const Color color
  );

public:
  const glm::mat4 TransformMatrix() const;
  const glm::mat4 ScaleMatrix() const;
  const glm::mat4 RotateMatrix() const;
  const int TextureId() const;
  const Color GetColor() const;

  void SetTexture(const std::string& texture_name);
  void SetColor(const Color color);

  virtual void render() = 0;

protected:

  void use_texture();

  int m_texture_id;
  struct Color m_color;
  bool m_use_color;

  glm::mat4 m_transformation;
  glm::mat4 m_rotation;
  glm::mat4 m_scale;
};

inline Shape::Shape(const glm::vec3& position)
  : Shape(
    glm::translate(glm::mat4(1.0f), position),
    glm::mat4(1.0f),
    glm::mat4(1.0f),
    -1,
    DEFAULT_COLOR
  )
{}

inline Shape::Shape(
  const glm::vec3& position,
  const Color color
)
  : Shape(position)
{
  m_color = color;
  m_use_color = true;
  m_texture_id = -1;
}

inline Shape::Shape(
    const glm::vec3& position,
    const std::string& texture_name
)
  : Shape(position)
{
  // Call set texture
  TextureStatus status = TextureManager::get_texture(
    texture_name
  );

  if(!status.success) {
    m_texture_id = -1;
    m_use_color = true;
    return;
  }

  m_texture_id = status.texture_id;
  m_use_color = false;
}

inline Shape::Shape(
  const glm::mat4& transformation,
  const glm::mat4& scale,
  const glm::mat4& rotation,
  const int texture_id,
  const Color color
)
  : m_transformation{transformation},
    m_scale{scale},
    m_rotation{rotation},
    m_texture_id{texture_id},
    m_color{color}
{
  if(m_texture_id == -1) {
    m_use_color = true;
  }
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

inline const int Shape::TextureId() const {
  return m_texture_id;
}

inline const Color Shape::GetColor() const {
  return m_color;
}

inline void Shape::use_texture() {
  // Handle textures
  unsigned int m_shader_id =
    Engine<OpenGLWrapper>::shader().get_program();
  if(!m_use_color) {
    unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
    if(m_texture_id == -1) {
      glUniform1i(use_texture_id, (int)false);
    } else {
      glUniform1i(use_texture_id, (int)true);

      // Probably not a good idea to bind every render call
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_texture_id);    
    }
  }

  // Handle color
  if(m_use_color) {
    unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
    glUniform1i(use_texture_id, (int)false);

    unsigned int color_id = glGetUniformLocation(m_shader_id, "color");

    float r = ((float)m_color.r) / 255;
    float g = ((float)m_color.g) / 255;
    float b = ((float)m_color.b) / 255;
    glUniform4f(color_id, r, g, b, 1.0);
  }
}

#endif //SHAPE_H_
