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

  void change_option(bool direction);

  int m_pointer_index = 0;

  std::shared_ptr<Cube> m_pointer;
  std::shared_ptr<PhraseBuilder> m_title;
  std::vector<PhraseBuilder> m_options;
};

#endif // EXPERIMENT_MAIN_MENU_H_
