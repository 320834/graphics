#ifndef EXPERIMENT_MAIN_MENU_H_
#define EXPERIMENT_MAIN_MENU_H_

#include <vector>

#include "experiment/scene.h"
#include "experiment/cube.h"
#include "experiment/char_3d.h"

class MainMenuScene : public SceneInterface {
public:
  MainMenuScene(
    const std::shared_ptr<Engine> engine,
    const std::string& scene_name
  );

  void render() override;
  void controls() override;

private:
  std::shared_ptr<PhraseBuilder> m_phrase;
};

#endif // EXPERIMENT_MAIN_MENU_H_
