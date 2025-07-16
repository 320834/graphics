#include "generic_scene.h"
#include "shapes/cube.h"
#include "shapes/dynamic.h"
#include "engine.h"
#include <glm/glm.hpp>

GenericScene::GenericScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{
  m_shapes.push_back(
    std::make_shared<Cube>(
      glm::vec3(0.0f, 0.0f, -4.0f),
      "../demo_game/textures/apple.jpg"
    )
  );

  // m_shapes.push_back(
  //   std::make_shared<Square>(
  //     glm::vec3(1.0f, 1.0f, -6.0f)
  //   )
  // );

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
  
  // TODO: Need to move definition in header.
  auto* glfw_window = m_engine->glfw_window();
  bool l = glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS;

  if(l) {
    auto shape = DynamicShapeManager::get_dyn_shape("another");

    if(shape) {
      m_shapes.push_back(
        std::make_shared<Dynamic>(
          "another",
          glm::vec3(1.0f, 1.0f, -5.0f)
        )
      );
    } else {
      bool success = DynamicShapeManager::load_shape("another", "../demo_game/shapes/another.txt"); 

      if(success) {
        m_shapes.push_back(
          std::make_shared<Dynamic>(
            "another",
            glm::vec3(1.0f, 1.0f, -10.0f)
          )
        );
      }
    }
  }
}
