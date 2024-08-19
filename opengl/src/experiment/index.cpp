#include <iostream>
#include <vector>
#include <memory>

#include "experiment/engine.h"
#include "experiment/utils.h"
#include "experiment/snake_scene.h"
#include "experiment/main_menu_scene.h"

int runner() {

  std::shared_ptr<Engine> engine =
    std::make_shared<Engine>(
      "Snake Game", 
      "../shaders/experiment/simple.vert",
      "../shaders/experiment/simple.frag",
      1920,
      1080
    );

  const std::string snake_scene_name =
    "snake_game";
  std::shared_ptr<SnakeScene> snake_game =
    std::make_shared<SnakeScene>(engine, snake_scene_name);

  const std::string menu_scene_name = "main_menu"; 
  std::shared_ptr<MainMenuScene> main_menu =
    std::make_shared<MainMenuScene>(engine, menu_scene_name);

  engine->scene_manager().add_scene(main_menu);
  engine->scene_manager().add_scene(snake_game);

  engine->add_event(
    snake_scene_name,
    "game_lose",
    [](Engine& engine) {
      utils::log("Terminate Game", "Engine Event");
      glfwSetWindowShouldClose(engine.glfw_window(), true);
    }
  );

  
  engine->add_event(
    menu_scene_name,
    "start",
    [&snake_scene_name](Engine& engine) {
      utils::log("Start Snake Game", "Engine Event");
      engine.scene_manager().set_current_scene(snake_scene_name);
    }
  );

  engine->add_event(
    menu_scene_name,
    "quit",
    [](Engine& engine) {
      utils::log("Quit", "Engine Event");
      glfwSetWindowShouldClose(engine.glfw_window(), true);
    }
  );

  // engine->scene_manager().set_current_scene(menu_scene_name);
  engine->loop();

  return 0;
}
