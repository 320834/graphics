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

  PhraseBuilder start(m_id, "Start", 0.1f);
  start.transform(glm::vec3(-1.5,-1,-15));
  
  m_options.push_back(start);
  
  PhraseBuilder quit(m_id, "Quit", 0.1f);
  quit.transform(glm::vec3(-1.5, -2.5, -15));

  m_options.push_back(quit);

  m_pointer = std::make_shared<Cube>(
    m_id, glm::vec3(-1.5, -1, -15), default_letter_color 
  );
}

void MainMenuScene::render() {
  m_title->render();

  m_pointer->Render();

  for(PhraseBuilder& phrase : m_options) {
    phrase.render();
  }
}

void MainMenuScene::controls() {

}

void MainMenuScene::change_option(bool direction) {
  // true is up
  // false is down

  int curr_i = m_pointer_index;
  int new_i = -1;
  if(direction) {
    new_i = (curr_i + 1) == m_options.size() ? 
      0 : 
      (curr_i + 1);
  } else {
    new_i = (curr_i - 1) < 0 ?
      m_options.size() - 1 :
      curr_i - 1;
  }

  PhraseBuilder curr = m_options[curr_i];
  PhraseBuilder new_option = m_options[new_i];

  glm::vec3 change = new_option.position() - curr.position();

  m_pointer->Transform(change);
}


