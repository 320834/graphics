#include "shapes/cube.h"
#include "engine.h"

#include <glad/glad.h>

namespace {
  const glm::vec3 empty(0.0f, 0.0f, 0.0f);
}

unsigned int Cube::m_VAO = 0;
unsigned int Cube::m_VBO = 0;
unsigned int Cube::m_stride = 5;
std::vector<VertexType> Cube::m_vertices = 
{
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // TopBackLeft
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // BottomFrontRight
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

// =================================
// Need to be called once for setup
void Cube::init_vertex_buffers() {
  glGenBuffers(1, &Cube::m_VBO);
  glGenVertexArrays(1, &Cube::m_VAO);

  glBindVertexArray(Cube::m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, Cube::m_VBO); 

  const int size = sizeof(VertexType) * Cube::m_vertices.size();
  glBufferData(GL_ARRAY_BUFFER, size, Cube::m_vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Cube::m_stride * sizeof(VertexType),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Cube::m_stride * sizeof(VertexType),
    (void*)(3 * sizeof(VertexType)));
  glEnableVertexAttribArray(1);
}

// =================================

Cube::Cube(const glm::vec3& position)
  : Shape(position) 
{}

Cube::Cube(
  const glm::vec3& position,
  const std::string& texture_name
)
  : Shape(position, texture_name)
{}

Cube::Cube(const Cube& other)
  : Shape(
      other.TransformMatrix(),
      other.RotateMatrix(),
      other.ScaleMatrix(),
      other.TextureId(),
      other.GetColor()
    )
{}

Cube& Cube::operator=(const Cube& other) {
  if(this == &other) {
    return *this;
  }

  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();

  return *this;
}

void Cube::render() {

  Shape::use_texture();

  Shader& simple_shader =
    Engine<OpenGLWrapper>::simple_shader();
  simple_shader.use();

  glBindVertexArray(Cube::m_VAO);

  simple_shader.setMat4("transform", m_transformation);
  simple_shader.setMat4("rotate", m_rotation);
  simple_shader.setMat4("scale", m_scale);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}