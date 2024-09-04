#include "experiment/snake_scene.h"

#include <unordered_set>
#include <random>

SnakeScene::SnakeScene(
  const std::shared_ptr<Engine> engine,
  const std::string& scene_name
)
  : SceneInterface(engine, scene_name),
    m_next_tick_last{std::chrono::system_clock::now()},
    m_rotate_time{std::chrono::system_clock::now()},
    m_tick_time{NEXT_TICK_MAXIMUM},
    m_score{0}
{
  init_snake_body();
  init_walls();
  spawn_food();
}

void SnakeScene::render() {
  std::chrono::time_point now =
    std::chrono::system_clock::now();
  auto duration =
    std::chrono::duration_cast<std::chrono::milliseconds>(now - m_next_tick_last);

  auto duration_rotate =
    std::chrono::duration_cast<std::chrono::seconds>(now - m_rotate_time);
  
  // Do rotate of board for spice. Runs every x seconds
  if(duration_rotate.count() >= 10) {
    bool keep_rotating = rotate_board();
    
    if(!keep_rotating) {
      utils::log("Finish Rotating", "SnakeGame");
      m_rotate_time = std::chrono::system_clock::now();
    }
  } 

  // Concept of next tick. Run every x milliseconds
  if(duration.count() >= m_tick_time) {
    m_next_tick_last = std::chrono::system_clock::now();
    move_snake_body();
    check_collisions();
  }

  m_tick_time =
    std::max(NEXT_TICK_MAXIMUM - ((size_t)(m_score / 5) * 25), NEXT_TICK_MINIMUM);


  for(
    unsigned int index = 0;
    index < m_snake.size();
    ++index
  ) {
      
    Cube& cube = m_snake[index];
    if(index == m_head_index) {
      cube.SetTexture("head.png");
    } else {
      cube.SetTexture("snake.jpg");
    }
    cube.Render();
  }

  for(Cube& wall : m_walls) {
    wall.SetTexture("container.jpg");
    wall.Render();
  }

  for(Cube& food : m_food) {
    food.Render();
  }
}

void SnakeScene::controls() {
  if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {

    if(m_yaw == 270.0f) {
      change_direction(SnakeDirection::UP);
    } else {
      change_direction(SnakeDirection::DOWN);
    }
  } 
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
    if(m_yaw == 270.0f) {
      change_direction(SnakeDirection::DOWN);
    } else {
      change_direction(SnakeDirection::UP);
    }
  } 
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    change_direction(SnakeDirection::LEFT);
  }
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    change_direction(SnakeDirection::RIGHT);
  }
}

void SnakeScene::init_snake_body() {
  m_snake.emplace_back(
    m_engine, glm::vec3(0.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    m_engine, glm::vec3(1.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    m_engine, glm::vec3(2.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    m_engine, glm::vec3(3.0f, 0.0f, m_z_depth), snake_body_color
  );
}

void SnakeScene::init_walls() {

  const std::string wall_texture = "container.jpg"; 
  
  // Construct left
  {
    for(int i = 0; i <= m_wall_length; ++i) {
      m_walls.emplace_back(m_engine, glm::vec3(-m_wall_length, i, m_z_depth), wall_texture);
    }

    for(int i = -1; i >= -m_wall_length; --i) {
      m_walls.emplace_back(m_engine, glm::vec3(-m_wall_length, i, m_z_depth), wall_texture);
    }
  }

  // Construct right
  {
    for(int i = 0; i <= m_wall_length; ++i) {
      m_walls.emplace_back(m_engine, glm::vec3(m_wall_length, i, m_z_depth), wall_texture);
    }

    for(int i = -1; i >= -m_wall_length; --i) {
      m_walls.emplace_back(m_engine, glm::vec3(m_wall_length, i, m_z_depth), wall_texture);
    }
  }

  // Construct top
  {
    for(int i = 0; i <= m_wall_length; ++i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, m_wall_length, m_z_depth), wall_texture);
    }

    for(int i = -1; i >= -m_wall_length; --i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, m_wall_length, m_z_depth), wall_texture);
    }
  }

  // Construct bottom
  {
    for(int i = 0; i <= m_wall_length; ++i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, m_wall_length, m_z_depth), wall_texture);
    }

    for(int i = -1; i >= -m_wall_length; --i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, m_wall_length, m_z_depth), wall_texture);
    }
  }

  // Construct top
  {
    for(int i = 0; i <= m_wall_length; ++i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, -m_wall_length, m_z_depth), wall_texture);
    }

    for(int i = -1; i >= -m_wall_length; --i) {
      m_walls.emplace_back(m_engine, glm::vec3(i, -m_wall_length, m_z_depth), wall_texture);
    }
  }
}

void SnakeScene::move_snake_body() {
  size_t tail_index = 
  m_head_index == 0 ? m_snake.size() - 1 :
  m_head_index - 1;

  if(m_head_index >= m_snake.size() ||
     tail_index >= m_snake.size())
  {
    return;
  }

  Cube& head_cube = m_snake[m_head_index];
  Cube& tail_cube = m_snake[tail_index];

  tail_cube.SetPosition(head_cube.GetPosition());

  // Determine direction here
  switch(m_curr_dir) {
    case SnakeDirection::LEFT:
      tail_cube.Transform(glm::vec3(-1.0f, 0.0f, 0.0f));
      break;
    case SnakeDirection::RIGHT:
      tail_cube.Transform(glm::vec3(1.0f, 0.0f, 0.0f));
      break;
    case SnakeDirection::DOWN:
      tail_cube.Transform(glm::vec3(0.0f, -1.0f, 0.0f));
      break;
    case SnakeDirection::UP:
      tail_cube.Transform(glm::vec3(0.0f, 1.0f, 0.0f));
      break;
    default:
      tail_cube.Transform(glm::vec3(-1.0f, 0.0f, 0.0f));
  }

  // Loop continue, update head index
  if(m_head_index == 0) {
    m_head_index = m_snake.size() - 1;
  } else {
    m_head_index -= 1;
  }
}

FoodCollision SnakeScene::has_eaten() {
  if(m_snake.size() == 0) {
    return {
      .collide = false,
      .index = -1
    };
  }

  Cube& head = m_snake[m_head_index];

  for(int index = 0; index < m_food.size(); ++index) {
    Cube::Collision col = m_food[index].IsColliding(head); 
 
    if(col.collide && col.points >= 8) {
      return {
        .collide = true,
        .index = index
      };
    }

  }

  return {
    .collide = false,
    .index = -1
  };
}

void SnakeScene::change_direction(SnakeDirection new_dir) {
  if(
    (m_curr_dir == SnakeDirection::LEFT &&
    new_dir == SnakeDirection::RIGHT) ||
    (m_curr_dir == SnakeDirection::UP &&
    new_dir == SnakeDirection::DOWN) ||
    (m_curr_dir == SnakeDirection::DOWN &&
    new_dir == SnakeDirection::UP) ||
    (m_curr_dir == SnakeDirection::RIGHT &&
    new_dir == SnakeDirection::LEFT)
  ) {
    return;
  }

  m_curr_dir = new_dir;
}

glm::vec3 SnakeScene::get_append_pos() {
  size_t tail_index = 
    m_head_index == 0 ? 
    m_snake.size() - 1 :
    m_head_index - 1;

  size_t second_last =
    tail_index == 0 ?
    m_snake.size() - 1 :
    tail_index - 1;

  Cube& tail_cube = m_snake[tail_index];
  Cube& second_last_cube = m_snake[second_last];
  
  glm::vec3 tail_pos = tail_cube.GetPosition();
  glm::vec3 sec_last_pos = second_last_cube.GetPosition();

  SnakeDirection append_dir = SnakeDirection::LEFT;

  // Same x must be different y
  if(tail_pos.x == sec_last_pos.x) {
    if(tail_pos.y < sec_last_pos.y) {
      append_dir = SnakeDirection::DOWN;
    } else {
      append_dir = SnakeDirection::UP;
    }
  } else if(tail_pos.y == sec_last_pos.y) {
    if(tail_pos.x < sec_last_pos.x) {
      append_dir = SnakeDirection::LEFT;
    } else {
      append_dir = SnakeDirection::RIGHT;
    }
  }

  glm::vec3 new_cube_pos(0,0,0);
  switch(append_dir) {
    case SnakeDirection::UP:
      new_cube_pos = tail_pos + glm::vec3(0.0f, 1.0f, 0.0f); 
      break;
    case SnakeDirection::DOWN:
      new_cube_pos = tail_pos + glm::vec3(0.0f, -1.0f, 0.0f);
      break;
    case SnakeDirection::LEFT:
      new_cube_pos = tail_pos + glm::vec3(-1.0f, 0.0f, 0.0f);
      break;
    case SnakeDirection::RIGHT:
      new_cube_pos = tail_pos + glm::vec3(1.0f, 0.0f, 0.0f);
      break;
    default:
      new_cube_pos = glm::vec3(0,0,0);
  }

  return new_cube_pos;
}

void SnakeScene::insert_body_end() {
  size_t insert_index = m_head_index;
  glm::vec3 new_cube_pos = get_append_pos();

  auto it = m_snake.begin() + insert_index;
  m_head_index = m_head_index + 1;

  Cube cube(m_engine, new_cube_pos, snake_body_color);
  m_snake.insert(it, cube);
}

std::vector<glm::vec3> SnakeScene::get_empty_positions() {
  std::unordered_set<std::string> filled;
  for(Cube& snake : m_snake) {
    const glm::vec3 pos = snake.GetPosition();
    std::string hash =
     utils::hash_vec_str(pos);
    
    filled.emplace(hash);
  }

  for(Cube& food : m_food) {
    const glm::vec3 pos = food.GetPosition();
    std::string hash =
      utils::hash_vec_str(pos);
    filled.emplace(hash);
  }

  std::vector<glm::vec3> empty_positions;
  for(int x = -m_wall_length + 1; x < m_wall_length; ++x)
  {
    for(int y = m_wall_length - 1; y > -m_wall_length; --y) 
    {
      const glm::vec3 potential(x, y, m_z_depth);

      std::string hash =
        utils::hash_vec_str(potential);

      if(filled.count(hash) == 0) {
         empty_positions.push_back(potential);
      }
    }
  }
  
  // const std::string message =
  // "Empty Positions: " + std::to_string(empty_positions.size());
  // utils::log(message, "SnakeGame");

  return empty_positions;
}

void SnakeScene::spawn_food() {
  std::vector<glm::vec3> positions =
    get_empty_positions();

  if(positions.size() == 0) {
    // Error or end game
    return;
  }
  
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, positions.size() - 1);
  int pos_i = dist(rng);

  const glm::vec3 pos = positions[pos_i];
  const int shader_id = m_engine->shader().m_ID;
  m_food.emplace_back(m_engine, pos, "apple.jpg");
}

void SnakeScene::check_collisions() {

  // Check if eat food, add to snake, and remove
  FoodCollision food_col = has_eaten();
  if(food_col.collide) {
    insert_body_end();
    int index_to_remove = food_col.index;

    m_food.erase(m_food.begin() + index_to_remove);
    spawn_food();

    ++m_score;

    utils::log("Head Eating. Score: " + std::to_string(m_score), "SnakeGame");
    utils::log("Next tick: " + std::to_string(m_tick_time), "SnakeGame");
  }

  // Check for wall collisions
  Cube& head = m_snake[m_head_index];
  for(Cube& wall : m_walls) {
    Cube::Collision col = wall.IsColliding(head);
    if(col.collide && col.points >= 8) {
      utils::log("Head Colliding Wall", "SnakeGame");
      m_engine->invoke_event(scene_name(), "game_lose");
    }
  }
  
  // Check for self collisions
  for(size_t i = 0; i < m_snake.size(); ++i) {
    
    Cube& snake_body = m_snake[i];
    // Do not check head
    if(i == m_head_index) {
      continue;
    }

    Cube::Collision col = head.IsColliding(snake_body);
    
    if(col.collide && col.points >= 8) {
      utils::log("Head Colliding Body", "SnakeGame");
      m_engine->invoke_event(scene_name(), "game_lose");
    }

  }
}

bool SnakeScene::rotate_board() {

  const float rotate_amount = 1;

  m_degree += rotate_amount;
  m_degree = (int)m_degree % 360;

  m_yaw += rotate_amount;
  m_yaw = (int)m_yaw % 360;

  int radius = ((-1) * m_z_depth);

  float x = radius * glm::cos(glm::radians(m_degree));
  float z = radius * glm::sin(glm::radians(m_degree)) - radius;
  
  m_engine->camera().Position = glm::vec3(x, 0.0f, z);
  
  int yaw = (int)m_engine->camera().Yaw % 360; 
  m_engine->camera().Yaw = m_yaw;
  m_engine->camera().UpdateCameraVectors();

  if(m_yaw == 90.0f || m_yaw == 270.0f) {
    return false;
  }

  return true;
}

void SnakeScene::reset() {
  m_next_tick_last = std::chrono::system_clock::now();
  m_rotate_time = std::chrono::system_clock::now();

  m_engine->reset_camera();
  m_tick_time = NEXT_TICK_MAXIMUM;
  m_curr_dir = SnakeDirection::LEFT;

  m_degree = 90;
  m_yaw = 270;
  
  m_head_index = 0;
  m_score = 0;

  m_food.clear();
  m_snake.clear();

  init_snake_body();

  spawn_food();
}
