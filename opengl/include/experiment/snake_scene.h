#ifndef EXPERIMENT_SNAKE_SCENE_H_
#define EXPERIMENT_SNAKE_SCENE_H_

#include "scene.h"

class SnakeScene : SceneInterface {

  SnakeScene() = default;

  void render() override;
  void controls() override;
};

#endif //EXPERIMENT_SNAKE_SCENE_H_
