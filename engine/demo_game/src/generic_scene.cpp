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
    std::make_shared<Dynamic>(
      "backpack",
      glm::vec3(0.0f, 0.0f, -10.0f)
    )
  );

  Color color = {
    .r = 233, .g = 100, .b = 55
  };

  m_shapes.push_back(
    std::make_shared<Cube>(
      glm::vec3(0.0f, 0.0f, -4.0f),
      "../demo_game/textures/apple.jpg"
    )
  );

  m_shapes.push_back(
    std::make_shared<Dynamic>(
      "backpack",
      glm::vec3(10.0f, 10.0f, -30.0f)
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
  bool p = glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS;

  if(p) {
    auto shape = DynamicShapeManager::get_dyn_shape("backpack");

    if(shape) {
      m_shapes.push_back(
        std::make_shared<Dynamic>(
          "backpack",
          glm::vec3(1.0f, 1.0f, -5.0f)
        )
      );
    }
  }

  float step = 0.5f;

  if(glfwGetKey(glfw_window, GLFW_KEY_I) == GLFW_PRESS) {
    m_shapes[2]->Translate(glm::vec3(0.0f, step, 0.0f));
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_K) == GLFW_PRESS) {
    m_shapes[2]->Translate(glm::vec3(0.0f, -step, 0.0f));
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_J) == GLFW_PRESS) {
    m_shapes[2]->Translate(glm::vec3(-step, 0.0f, 0.0f));
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS) {
    m_shapes[2]->Translate(glm::vec3(step, 0.0f, 0.0f));
  }
}
