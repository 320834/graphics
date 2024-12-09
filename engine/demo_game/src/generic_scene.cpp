#include "generic_scene.h"
#include "shapes/cube.h"
#include "shapes/square.h"
#include "shapes/dynamic.h"
#include <glm/glm.hpp>

GenericScene::GenericScene(const std::string& scene_name)
  : SceneInterface(scene_name)
{
  m_shapes.push_back(
    std::make_shared<Cube>(
      glm::vec3(0.0f, 0.0f, -4.0f)
    )
  );

  m_shapes.push_back(
    std::make_shared<Square>(
      glm::vec3(1.0f, 1.0f, -6.0f)
    )
  );

  m_shapes.push_back(
    std::make_shared<Dynamic>(
      "cube_dynamic",
      glm::vec3(-1.0f, -1.0f, -4.0f)
    )
  );

  m_shapes.push_back(
    std::make_shared<Dynamic>(
      "weird_dynamic",
      glm::vec3(-1.0f, -1.0f, -10.0f)
    )
  );
}

void GenericScene::render() {
  for(std::shared_ptr<Shape>& shape : m_shapes) {
    shape->render();
  }
}

void GenericScene::controls() {

}