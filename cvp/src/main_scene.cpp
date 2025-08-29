#include "main_scene.h"

#include "shapes/cube.h"
#include "shapes/dynamic.h"
#include "engine.h"
#include "utils.h"
#include <glm/glm.hpp>

MainScene::MainScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{

  m_engine->set_background({0.0f, 0.0f, 0.0f});

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
    std::make_shared<Dynamic>(
      "backpack",
      glm::vec3(10.0f, 10.0f, -30.0f)
    )
  );

  directional_light().set_direction(glm::vec3(-0.2f, -1.0f, -0.3f));
  directional_light().set_ambient(glm::vec3(0.1f, 0.1f, 0.1f));
  directional_light().set_diffuse(glm::vec3(0.01f, 0.01f, 0.01f));
  directional_light().set_specular(glm::vec3(0.5f, 0.5f, 0.5f));

  std::shared_ptr<PointLight> point_one =
    add_point_light(glm::vec3(0.0f, 0.0f, 0.0f));

  point_one->set_ambient(glm::vec3(0.1f, 0.1f, 0.1f));
  point_one->set_diffuse(glm::vec3(0.1f, 0.1f, 0.1f));
  point_one->set_specular(glm::vec3(1.0f, 1.0f, 1.0f));

  std::shared_ptr<SpotLight> spot_one =
    add_spot_light(glm::vec3(0.0f, 0.0f, -2.0f));
  spot_one->set_ambient(glm::vec3(0.0f, 0.0f, 0.0f));
  spot_one->set_diffuse(glm::vec3(1.0f, 1.0f, 1.0f));
  spot_one->set_specular(glm::vec3(0.5f, 0.5f, 0.5f));
  spot_one->set_cutoffs(2.5f, 5.0f);

  auto cube = std::make_shared<Cube>(
    glm::vec3(spot_one->position()),
    "../demo_game/textures/apple.jpg"
  );

  cube->Scale(0.2);

  m_shapes.push_back(cube);

}

void MainScene::render() {

  for(std::shared_ptr<Shape>& shape : m_shapes) {
    shape->render();
  }
}

void MainScene::controls() {
  // TODO: Need to move definition in header.
  auto* glfw_window = m_engine->glfw_window();
  bool m = glfwGetKey(glfw_window, GLFW_KEY_M) == GLFW_PRESS;

  if(m) {
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

  float step = 0.1f;

  std::shared_ptr<Shape> cube =
    m_shapes.back();

  if(glfwGetKey(glfw_window, GLFW_KEY_I) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(0.0f, step, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(0.0f, step, 0.0f);
    // cube->Translate(glm::vec3(0.0f, step, 0.0f));
    
    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(0.0f, step, 0.0f);
    cube->Translate(glm::vec3(0.0f, step, 0.0f));

    m_spot_lights[0]->set_position(pos);
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_K) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(0.0f, -step, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(0.0f, -step, 0.0f);
    // cube->Translate(glm::vec3(0.0f, -step, 0.0f));

    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(0.0f, -step, 0.0f);
    cube->Translate(glm::vec3(0.0f, -step, 0.0f));

    m_spot_lights[0]->set_position(pos);
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_J) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(-step, 0.0f, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(-step, 0.0f, 0.0f);
    // cube->Translate(glm::vec3(-step, 0.0f, 0.0f));

    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(-step, 0.0f, 0.0f);
    cube->Translate(glm::vec3(-step, 0.0f, 0.0f));

    m_spot_lights[0]->set_position(pos);
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(step, 0.0f, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(step, 0.0f, 0.0f);
    // cube->Translate(glm::vec3(step, 0.0f, 0.0f));

    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(step, 0.0f, 0.0f);
    cube->Translate(glm::vec3(step, 0.0f, 0.0f));

    m_spot_lights[0]->set_position(pos);
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_O) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(step, 0.0f, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(0.0f, 0.0f, step);
    // cube->Translate(glm::vec3(0.0f, 0.0f, step));

    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(0.0f, 0.0f, step);
    cube->Translate(glm::vec3(0.0f, 0.0f, step));

    m_spot_lights[0]->set_position(pos);
  }

  if(glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS) {
    // m_shapes[2]->Translate(glm::vec3(step, 0.0f, 0.0f));

    // glm::vec3 pos = m_point_lights[0]->position();
    // pos += glm::vec3(0.0f, 0.0f, -step);
    // cube->Translate(glm::vec3(0.0f, 0.0f, -step));

    // m_point_lights[0]->set_position(pos);

    glm::vec3 pos = m_spot_lights[0]->position();
    pos += glm::vec3(0.0f, 0.0f, -step);
    cube->Translate(glm::vec3(0.0f, 0.0f, -step));

    m_spot_lights[0]->set_position(pos);
  }
}
