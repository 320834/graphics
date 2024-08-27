#include "experiment/game_end_scene.h"

#include "experiment/engine.h"
#include "experiment/char_3d.h"

GameEndScene::GameEndScene(
  std::shared_ptr<Engine> engine,
  const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{
  const unsigned int id = m_engine->shader().m_ID;
  m_is_win = false; 

  m_lose = std::make_shared<PhraseBuilder>(
    id, "You Lose", 0.35f 
  );
  m_win = std::make_shared<PhraseBuilder>(
    id, "You Win", 0.35f
  );
  m_lose->transform(glm::vec3(-8.5,1,-15));
  m_win->transform(glm::vec3(-7,1,-15));

  PhraseBuilder menu(id, "Menu", 0.1f);
  menu.transform(glm::vec3(-1.5,-1,-15));
  
  m_options.push_back(menu);
  
  PhraseBuilder quit(id, "Quit", 0.1f);
  quit.transform(glm::vec3(-1.5, -2.5, -15));

  m_options.push_back(quit);

  m_pointer = std::make_shared<Cube>(
    id, glm::vec3(-2.5, -0.8, -15), default_letter_color 
  );

  m_pointer->ScaleX(0.8f);
  m_pointer->ScaleY(0.1f);
  m_pointer->ScaleZ(0.1f);


  m_next_tick_last = std::chrono::system_clock::now();

}

void GameEndScene::render() {
  if(m_is_win) {
    m_win->render();
  } else {
    m_lose->render();
  }

  m_pointer->Render();

  for(PhraseBuilder& option : m_options) {
    option.render();
  }
}

void GameEndScene::controls() {
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

    
    if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_ENTER) == GLFW_PRESS) {
      hit_option();
      m_next_tick_last = std::chrono::system_clock::now();
    }
  }
  
}

void GameEndScene::set_condition(bool is_win) {
  m_is_win = is_win;
}

void GameEndScene::change_option(bool direction) {
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

void GameEndScene::hit_option() {
  if(m_pointer_index == 0) {
    m_engine->invoke_event(
      scene_name(), "menu" 
    );
  } else if(m_pointer_index == 1) {
    m_engine->invoke_event(
      scene_name(), "quit"
    );
  }
}

