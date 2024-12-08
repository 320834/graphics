#ifndef GENERIC_SCENE_H_
#define GENERIC_SCENE_H_

#include "scene.h"
#include "shapes/shape.h"

class GenericScene : public SceneInterface {
public:
  GenericScene(const std::string& scene_name);
  ~GenericScene() = default;

  void render() override;
  void controls() override;

  std::vector<std::shared_ptr<Shape>> m_shapes;
};

#endif //GENERIC_SCENE_H_
