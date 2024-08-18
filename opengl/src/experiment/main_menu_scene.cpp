#include "experiment/main_menu_scene.h"
#include "experiment/char_3d.h"

#include <memory>

MainMenuScene::MainMenuScene(
  const std::shared_ptr<Engine> engine,
  const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{
  const unsigned int m_id = m_engine->shader().m_ID;

  m_title = std::make_shared<PhraseBuilder>(
    m_id, "Snake", 0.35f 
  );
  m_title->transform(glm::vec3(-5.5,1,-15));

  m_start = std::make_shared<PhraseBuilder>(
    m_id, "Start", 0.1f 
  );
  m_start->transform(glm::vec3(-2, -1,-15));

  m_quit = std::make_shared<PhraseBuilder>(
    m_id, "Quit", 0.1f 
  );
  m_quit->transform(glm::vec3(-2, -2.5,-15));
}

void MainMenuScene::render() {
  m_title->render();
  m_start->render();
  m_quit->render();
}

void MainMenuScene::controls() {

}
