#include "scene_manager.h"
#include "utils.h"

#include <string>

std::shared_ptr<SceneInterface> SceneManager::get_current_scene() { 
  return m_scenes.at(m_current_active_scene);
}

void SceneManager::set_current_scene(
  const std::string& scene_name
) {
  if(
    auto search = m_scenes.find(scene_name);
    search != m_scenes.end()
  ) {
    m_current_active_scene = scene_name;
  } else {
    utils::log(
      "Failed to set scene (not found): " + scene_name,
      "SceneManager"
    );
  }

  // Do nothing if it cannot set. Maybe I should
  // throw it
}

std::string SceneManager::get_current_scene_name()
{
  return m_current_active_scene;
}

bool SceneManager::add_scene(
  std::shared_ptr<SceneInterface> new_scene
)
{
  const std::string scene_name =
    new_scene->scene_name();

  bool has_inserted = false;
  if(
    auto search = m_scenes.find(scene_name);
    search == m_scenes.end() // If not found
  ) {
    m_scenes.insert({scene_name, new_scene});
    has_inserted = true;
  } else {
    utils::log("Scene Name Already Added: " + scene_name, "SceneManager");
  }

  if(m_scenes.size() == 1) {
    set_current_scene(scene_name);
  }

  return has_inserted;
}

void SceneManager::delete_scene(
  const std::string& scene_name
)
{
  if(scene_name == m_current_active_scene) {
    utils::log(
      "Failed To Delete Scene " + scene_name + ": Scene is active",
      "SceneManager"
    );

    return;
  }

  if(
    auto search = m_scenes.find(scene_name);
    search != m_scenes.end()
  ) {
    m_scenes.erase(scene_name);
  }
}