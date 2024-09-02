#include <iostream>
#include <vector>
#include <memory>

#include "experiment/engine.h"
#include "experiment/utils.h"
#include "experiment/snake_scene.h"
#include "experiment/main_menu_scene.h" 
#include "experiment/game_end_scene.h"

int runner() {

  std::shared_ptr<Engine> engine =
    create_engine(
      "Snake Game", 
      "../shaders/experiment/simple.vert",
      "../shaders/experiment/simple.frag",
      960,
      540
    );

  // 960,
  // 540

  const std::string snake_scene_name =
    "snake_game";
  std::shared_ptr<SnakeScene> snake_game =
    std::make_shared<SnakeScene>(engine, snake_scene_name);

  const std::string menu_scene_name = "main_menu"; 
  std::shared_ptr<MainMenuScene> main_menu =
    std::make_shared<MainMenuScene>(engine, menu_scene_name);

  const std::string game_end_scene_name = "end_menu";
  std::shared_ptr<GameEndScene> end_menu =
    std::make_shared<GameEndScene>(engine, game_end_scene_name);

  engine->scene_manager().add_scene(main_menu);
  engine->scene_manager().add_scene(snake_game);
  engine->scene_manager().add_scene(end_menu);

  engine->add_event(
    snake_scene_name,
    "game_lose",
    [&game_end_scene_name, &snake_scene_name](std::shared_ptr<Engine> engine) {
      utils::log("Game Lose Go Back To Main Menu", "Engine Event");

      engine->scene_manager().set_current_scene(game_end_scene_name);

      engine->scene_manager().delete_scene(snake_scene_name);
      std::shared_ptr<SnakeScene> snake_game =
        std::make_shared<SnakeScene>(engine, snake_scene_name);
      engine->scene_manager().add_scene(snake_game);

    }
  );

  
  engine->add_event(
    menu_scene_name,
    "start",
    [&snake_scene_name](std::shared_ptr<Engine> engine) {
      utils::log("Start Snake Game", "Engine Event");


      engine->scene_manager().set_current_scene(snake_scene_name);
    }
  );

  engine->add_event(
    menu_scene_name,
    "quit",
    [](std::shared_ptr<Engine> engine){
      utils::log("Quit", "Engine Event");
      glfwSetWindowShouldClose(engine->glfw_window(), true);
    }
  );

  engine->add_event(
    game_end_scene_name,
    "menu",
    [&menu_scene_name, &main_menu](std::shared_ptr<Engine> engine) {
      utils::log("Go to Menu", "Engine Event");
      engine->scene_manager().set_current_scene(menu_scene_name);
      main_menu->reset_timer();
    }
  );

  engine->add_event(
    game_end_scene_name,
    "quit",
    [](std::shared_ptr<Engine> engine) {
      utils::log("Quit", "Engine Event");
      glfwSetWindowShouldClose(engine->glfw_window(), true);
    }
  );

  // engine->scene_manager().set_current_scene(game_end_scene_name);
  
  engine->scene_manager().set_current_scene(menu_scene_name);
  engine->loop();

  return 0;
}
