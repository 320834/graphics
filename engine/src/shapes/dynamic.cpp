#include "shapes/dynamic.h"
#include "engine.h"
#include "shapes/assimp/model.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

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

  if (
    auto search = DynamicShapeManager::m_dynamic_shapes.find(dyn_shape_name);
    search != DynamicShapeManager::m_dynamic_shapes.end()
  ) {
    // Already loaded.
    return true;
  }  

  try {

    DynamicShapeMetadata data = {
      .name = dyn_shape_name,
      .filename = filename,
      .model = std::make_unique<Model>(filename)
    };

    m_dynamic_shapes.emplace(
      std::make_pair(
      dyn_shape_name, data));

    utils::log(
      "Sucessfully Loaded Model: " + filename + " (" + dyn_shape_name + ")",
      "Dynamic"
    );

    return true;

  } catch(std::exception e) {
    utils::log(
      "Failed to load: " + filename,
      "Dynamic"
    );

    return false;
  }

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
  if(name.empty()) {
    throw std::runtime_error("Empty dynamic file");
  }

  auto metadata = DynamicShapeManager::get_dyn_shape(name);

  if(!metadata) {
    throw std::runtime_error("No dynamic found with name: " + name);
  }

  m_model_name = metadata->name;
  m_model = metadata->model;
}

Dynamic::Dynamic(const Dynamic& other)
  : Shape(
      other.TransformMatrix(),
      other.RotateMatrix(),
      other.ScaleMatrix()
  )
{}

Dynamic& Dynamic::operator=(const Dynamic& other) {
  if(this == &other) {
    return *this;
  }

  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();

  return *this;
}

void Dynamic::render() {

  Shader& complex_shader =
    Engine<OpenGLWrapper>::assimp_shader();

  complex_shader.use();
  complex_shader.setMat4("transform", m_transformation);
  complex_shader.setMat4("rotate", m_rotation);
  complex_shader.setMat4("scale", m_scale);

  m_model->Render(complex_shader);
}
