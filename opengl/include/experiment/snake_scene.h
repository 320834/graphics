#ifndef EXPERIMENT_SNAKE_SCENE_H_
#define EXPERIMENT_SNAKE_SCENE_H_

#include "scene.h"
#include "experiment/cube.h"

#include <chrono>

enum class SnakeDirection {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT
};

struct FoodCollision {
  bool collide;
  int index;
};

const Color snake_body_color = {
  .r = 76,
  .g = 235,
  .b = 52
};

const Color snake_head_color = {
  .r = 229,
  .g = 235,
  .b = 52
};

const Color food_color = {
  .r = 255,
  .g = 0,
  .b = 0
};

const Color border_color = {
  .r = 0,
  .g = 0,
  .b = 0
};

const size_t NEXT_TICK_MAXIMUM = 500;
const size_t NEXT_TICK_MINIMUM = 50;

class SnakeScene : public SceneInterface {
public:
  SnakeScene(
    const std::shared_ptr<Engine> engine,
    const std::string& scene_name);

  void render() override;
  void controls() override;
private:

  void init_snake_body();
  void init_walls();

  void move_snake_body();

  FoodCollision has_eaten();
  void change_direction(SnakeDirection new_dir);
  glm::vec3 get_append_pos();
  void insert_body_end();

  std::vector<glm::vec3> get_empty_positions();
  void spawn_food();
  void check_collisions();

  // ==================================
  // Members
  
  size_t m_tick_time;
  size_t m_score;

  std::chrono::time_point<std::chrono::system_clock> m_next_tick_last;

  size_t m_head_index = 0;
  size_t m_max_food = 1;
  float m_wall_length = 6.0f;
  float m_z_depth = -15.0f; 
  SnakeDirection m_curr_dir = SnakeDirection::LEFT;

  std::vector<Cube> m_food;
  std::vector<Cube> m_walls;
  std::vector<Cube> m_snake;
};

#endif //EXPERIMENT_SNAKE_SCENE_H_
