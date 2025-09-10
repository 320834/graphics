#include "shapes/cube.h"
#include "engine.h"
#include "texture_manager.h"

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

Cube::Cube(
  const glm::vec3& position,
  const Color color
)
  : Shape(position)
{
  m_color = color;
  m_use_color = true;
  m_texture_id = -1;
}

Cube::Cube(const Cube& other)
  : Shape(
      other.TransformMatrix(),
      other.RotateMatrix(),
      other.ScaleMatrix()
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

const int Cube::TextureId() const {
  return m_texture_id;
}

const Color Cube::GetColor() const {
  return m_color;
}

void Cube::SetTexture(const std::string& texture_name) {
  TextureStatus status = TextureManager::get_texture(texture_name);

  if(!status.success) {
    m_texture_id = -1;
    m_use_color = true;
    return;
  }

  m_texture_id = status.texture_id;
  m_use_color = false;
}

void Cube::SetColor(const Color color) {
  m_use_color = true;
  m_color = color;
}

void Cube::use_texture() {
  // Handle textures
  unsigned int m_shader_id =
    Engine<OpenGLWrapper>::simple_shader().get_program();
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

void Cube::render() {

  Shader& simple_shader =
    Engine<OpenGLWrapper>::simple_shader();
  simple_shader.use();

  glBindVertexArray(Cube::m_VAO);

  use_texture();

  simple_shader.setMat4("transform", m_transformation);
  simple_shader.setMat4("rotate", m_rotation);
  simple_shader.setMat4("scale", m_scale);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
