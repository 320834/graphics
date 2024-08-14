#ifndef EXPERIMENT_SCENE_H_
#define EXPERIMENT_SCENE_H_

#include <memory>
#include "experiment/engine.h"

class SceneInterface {
public:
  virtual void render() = 0;
  virtual void controls() = 0;

  void set_engine(const std::shared_ptr<Engine>& engine);

protected:
  std::shared_ptr<Engine> m_engine;
};

inline void SceneInterface::set_engine(
  const std::shared_ptr<Engine>& engine)
{
  m_engine = engine;
}

#endif // EXPERIMENT_SCENE_H_
