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
  m_next_tick_last = std::chrono::system_clock::now();

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
    m_id, glm::vec3(-2.5, -0.8, -15), default_letter_color 
  );

  m_pointer->ScaleX(0.8f);
  m_pointer->ScaleY(0.1f);
  m_pointer->ScaleZ(0.1f);
}

void MainMenuScene::render() {

  m_title->render();

  m_pointer->Render();

  for(PhraseBuilder& phrase : m_options) {
    phrase.render();
  }
}

void MainMenuScene::controls() {

  std::chrono::time_point now =
    std::chrono::system_clock::now();
  auto duration =
    std::chrono::duration_cast<std::chrono::milliseconds>(now - m_next_tick_last);
 
  if(duration.count() >= 150) {

    if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {
      change_option(false);
      m_next_tick_last = std::chrono::system_clock::now();
    }

    if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      change_option(true);
      m_next_tick_last = std::chrono::system_clock::now();
    }

    
    if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      m_next_tick_last = std::chrono::system_clock::now();
    }
  }
}

void MainMenuScene::change_option(bool direction) {
  // true is up
  // false is down

  int new_i = -1;
  if(direction) {
    new_i = (m_pointer_index + 1) == m_options.size() ? 
      0 : 
      (m_pointer_index + 1);
  } else {
    new_i = (m_pointer_index - 1) < 0 ?
      m_options.size() - 1 :
      m_pointer_index - 1;
  }

  PhraseBuilder curr = m_options[m_pointer_index];
  PhraseBuilder new_option = m_options[new_i];

  glm::vec3 change = new_option.position() - curr.position();

  m_pointer->Transform(change);

  m_pointer_index = new_i;
}

void MainMenuScene::hit_option() {
  if(m_pointer_index == 0) {
    m_engine->invoke_event(scene_name(), "start"); 
  } else if(m_pointer_index == 1) {
    m_engine->invoke_event(scene_name(), "quit");
  }
}
