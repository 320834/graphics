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

  m_phrase = std::make_shared<PhraseBuilder>(
    m_id, "Snake", 0.35f 
  );

  m_phrase->transform(glm::vec3(-5,1,-15));
}

void MainMenuScene::render() {
  m_phrase->render();
}

void MainMenuScene::controls() {

}
