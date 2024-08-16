#ifndef EXPERIMENT_SCENE_H_
#define EXPERIMENT_SCENE_H_

#include <memory>
#include "experiment/engine.h"

class SceneInterface {
public:
  SceneInterface(
    const std::shared_ptr<Engine>& engine,
    const std::string& scene_name
  )
    : m_engine{engine},
      m_scene_name{scene_name}
  {}

  std::string scene_name() const;

  virtual void render() = 0;
  virtual void controls() = 0;

protected:
  std::shared_ptr<Engine> m_engine;
  std::string m_scene_name; // unique identifier
};

inline std::string SceneInterface::scene_name() const {
  return m_scene_name;
}

#endif // EXPERIMENT_SCENE_H_
