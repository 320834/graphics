#include "shapes/dynamic.h"
#include "engine.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace {
  const std::string empty_file;
  const glm::vec3 empty(0.0f, 0.0f, 0.0f);
}

// DynamicShapeManager Static
// ===================================
std::unordered_map<std::string, DynamicShapeMetadata>
  DynamicShapeManager::m_dynamic_shapes;

bool DynamicShapeManager::load_shape(
  const std::string& dyn_shape_name,
  const std::string& filename
) {
  std::ifstream file(filename);

  if (
    auto search = DynamicShapeManager::m_dynamic_shapes.find(dyn_shape_name);
    search != DynamicShapeManager::m_dynamic_shapes.end()
  ) {
    return false;
  }

  if (!file.is_open()) {
    std::cerr << "Failed to open the file." << std::endl;
    return false;
  }

  std::vector<VertexType> data;

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string token;
    
    while (std::getline(iss, token, ',')) {
      try {
        float value = std::stof(token);
        data.push_back(value);
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid float value: " << token << std::endl;
      } catch (const std::out_of_range& e) {
        std::cerr << "Float value out of range: " << token << std::endl;
      }
    }
  }

  file.close();

  DynamicShapeMetadata new_shape = {
    .m_VAO = 0,
    .m_VBO = 0,
    .m_stride = 5,
    .m_vertices = data
  };

  DynamicShapeManager::m_dynamic_shapes.insert(
    {dyn_shape_name, new_shape}
  );

  if (
    auto search = DynamicShapeManager::m_dynamic_shapes.find(dyn_shape_name);
    search != DynamicShapeManager::m_dynamic_shapes.end()
  ) {

    unsigned int& vao = search->second.m_VAO;
    unsigned int& vbo = search->second.m_VBO;
    unsigned int& stride = search->second.m_stride;
    std::vector<VertexType> vertices = search->second.m_vertices;

    glGenBuffers(1, &vao);
    glGenVertexArrays(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 

    const int size = sizeof(VertexType) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(VertexType),
      (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(VertexType),
      (void*)(3 * sizeof(VertexType)));
    glEnableVertexAttribArray(1);

    return true;
  }

  return false;
}

std::optional<DynamicShapeMetadata> DynamicShapeManager::get_dyn_shape(
  const std::string& dyn_shape_name
)
{
  if (
    auto search = DynamicShapeManager::m_dynamic_shapes.find(dyn_shape_name);
    search != DynamicShapeManager::m_dynamic_shapes.end()
  ) {
    return search->second;
  }

  return std::nullopt;
}

// ===================================

Dynamic::Dynamic(const std::string& name) 
  : Dynamic(name, empty)
{}

Dynamic::Dynamic(
  const std::string& name,
  const glm::vec3& position
) : Shape(position)
{
  auto shape = DynamicShapeManager::get_dyn_shape(name);
  if(shape) {
    m_VAO = shape->m_VAO;
    m_VBO = shape->m_VBO;
    return;
  }

  if(name.empty()) {
    throw std::runtime_error("Empty dynamic file");  
  } else {
    throw std::runtime_error("No dynamic found with name: " + name);
  }
}

Dynamic::Dynamic(const Dynamic& other)
  : Shape(
      other.TransformMatrix(),
      other.RotateMatrix(),
      other.ScaleMatrix(),
      other.TextureId(),
      other.GetColor()
  ),
  m_VAO(other.VAO()),
  m_VBO(other.VBO())
{}

Dynamic& Dynamic::operator=(const Dynamic& other) {
  if(this == &other) {
    return *this;
  }

  m_VAO = other.VAO();
  m_VBO = other.VBO();

  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();

  return *this;
}

unsigned Dynamic::VAO() const {
  return m_VAO;
}

unsigned Dynamic::VBO() const {
  return m_VBO;
}

void Dynamic::render() {
  glBindVertexArray(m_VAO);

  Shader& simple_shader =
    Engine<OpenGLWrapper>::simple_shader();
  simple_shader.use();

  simple_shader.setMat4("transform", m_transformation);
  simple_shader.setMat4("rotate", m_rotation);
  simple_shader.setMat4("scale", m_scale);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}
