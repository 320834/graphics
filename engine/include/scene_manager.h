#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "scene.h"
#include <unordered_map>

class SceneManager {
public:
  SceneManager() = default;
  ~SceneManager() = default;

  std::shared_ptr<SceneInterface> get_current_scene();
  void set_current_scene(const std::string& scene_name);
  std::string get_current_scene_name();

  bool add_scene(
    std::shared_ptr<SceneInterface> new_scene
  );
  void delete_scene(const std::string& scene_name);

private:
  std::string m_current_active_scene;
  std::unordered_map<std::string, std::shared_ptr<SceneInterface>>
    m_scenes;
};

#endif //SCENE_MANAGER_H_
