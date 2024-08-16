#ifndef EXPERIMENT_SNAKE_SCENE_H_
#define EXPERIMENT_SNAKE_SCENE_H_

#include "scene.h"

class SnakeScene : SceneInterface {

  SnakeScene(
    const std::shared_ptr<Engine> engine,
    const std::string& scene_name)
    : SceneInterface(engine, scene_name) 
  {};

  void render() override;
  void controls() override;
};

#endif //EXPERIMENT_SNAKE_SCENE_H_
