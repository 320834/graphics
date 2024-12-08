#include "shapes/square.h"
#include "engine.h"

#include <glad/glad.h>

namespace {
  const glm::vec3 empty(0.0f, 0.0f, 0.0f);
}

unsigned int Square::m_VAO = 0;
unsigned int Square::m_VBO = 0;
unsigned int Square::m_stride = 5;
std::vector<VertexType> Square::m_vertices = 
{
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // TopBackLeft
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f
};

// =================================
// Need to be called once for setup
void Square::init_vertex_buffers() {
  glGenBuffers(1, &Square::m_VBO);
  glGenVertexArrays(1, &Square::m_VAO);

  glBindVertexArray(Square::m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, Square::m_VBO); 

  const int size = sizeof(VertexType) * Square::m_vertices.size();
  glBufferData(GL_ARRAY_BUFFER, size, Square::m_vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Square::m_stride * sizeof(VertexType),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Square::m_stride * sizeof(VertexType),
    (void*)(3 * sizeof(VertexType)));
  glEnableVertexAttribArray(1);
}

// =================================

Square::Square()
  : Square(empty) 
{}

Square::Square(const glm::vec3& position)
  : Shape(position) 
{}

Square::Square(const Square& other)
  : Shape(
      other.TransformMatrix(),
      other.RotateMatrix(),
      other.ScaleMatrix()
    )
{}

Square& Square::operator=(const Square& other) {
  if(this == &other) {
    return *this;
  }

  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();

  return *this;
}

void Square::render() {
  glBindVertexArray(Square::m_VAO);

  unsigned int m_shader_id =
    Engine<OpenGLWrapper>::shader().get_program();
  unsigned int model_id_trans = glGetUniformLocation(m_shader_id, "transform");
  unsigned int model_id_rotate = glGetUniformLocation(m_shader_id, "rotate");
  unsigned int model_id_scale = glGetUniformLocation(m_shader_id, "scale");

  glUniformMatrix4fv(model_id_trans, 1, GL_FALSE, glm::value_ptr(m_transformation));
  glUniformMatrix4fv(model_id_rotate, 1, GL_FALSE, glm::value_ptr(m_rotation));
  glUniformMatrix4fv(model_id_scale, 1, GL_FALSE, glm::value_ptr(m_scale));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}