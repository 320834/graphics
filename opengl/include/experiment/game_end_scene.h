#ifndef EXPERIMENT_GAME_END_SCENE_H_
#define EXPERIMENT_GAME_END_SCENE_H_

#include "experiment/scene.h"
#include "experiment/engine.h"

#include "experiment/char_3d.h"

#include <chrono>

class GameEndScene : public SceneInterface {
public:
  GameEndScene(
    std::shared_ptr<Engine> engine,
    const std::string& scene_name
  );

  void render() override;
  void controls() override;

  void set_condition(bool is_win);
private:

  void change_option(bool direction);
  void hit_option();

  std::chrono::time_point<std::chrono::system_clock> m_next_tick_last;
  bool m_is_win;
  int m_pointer_index = 0;
 
  std::vector<PhraseBuilder> m_options;
  std::shared_ptr<Cube> m_pointer; 
  std::shared_ptr<PhraseBuilder> m_lose;
  std::shared_ptr<PhraseBuilder> m_win;
};

#endif // EXPERIMENT_GAME_END_SCENE_H_
