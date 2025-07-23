#include "engine.h"
#include "scene.h"
#include "constants.h"
#include "utils.h"

void SceneInterface::set_lights() {
  Shader& assimp_shader = m_engine->assimp_shader();
  
  assimp_shader.use();
  assimp_shader.setInt("point_lights_size", m_point_lights.size());
  assimp_shader.setInt("spot_lights_size", m_spot_lights.size());

  m_directional_light.set_light(assimp_shader);

  for(
    const std::shared_ptr<PointLight> point : 
    m_point_lights
  ) {
    point->set_light(assimp_shader);
  }

  for(
    const std::shared_ptr<SpotLight> spot :
    m_spot_lights
  ) {
    spot->set_light(assimp_shader);
  }
}

void SceneInterface::base_render() {
  set_lights();

  // User defined render
  render();
}

DirectionalLight& SceneInterface::directional_light()
{
  return m_directional_light;
}

std::shared_ptr<PointLight> SceneInterface::add_point_light(
  const glm::vec3 position
) {
  if(m_point_lights.size() >= constants::MAX_POINT_LIGHTS) {
    return nullptr;
  }

  const unsigned int index = m_point_lights.size();
  m_point_lights.emplace_back(
    std::make_shared<PointLight>(index, position));

  return m_point_lights[m_point_lights.size() - 1];
}

std::shared_ptr<SpotLight> SceneInterface::add_spot_light(
  const glm::vec3 position
) {
  if(m_spot_lights.size() >= constants::MAX_POINT_LIGHTS) {
    return nullptr;
  }

  const unsigned int index = m_spot_lights.size();
  m_spot_lights.emplace_back(
    std::make_shared<SpotLight>(index, position));

  return m_spot_lights[m_spot_lights.size() - 1];
}

std::string SceneInterface::scene_name() const {
  return m_scene_name;
}