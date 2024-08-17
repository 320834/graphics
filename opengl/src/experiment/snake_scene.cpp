#include "experiment/snake_scene.h"

#include <unordered_set>

SnakeScene::SnakeScene(
  const std::shared_ptr<Engine> engine,
  const std::string& scene_name
)
  : SceneInterface(engine, scene_name),
    m_next_tick_last{std::chrono::system_clock::now()}
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

  // Concept of next tick. Run every x milliseconds
  if(duration.count() >= 500) {
    m_next_tick_last = std::chrono::system_clock::now();

    move_snake_body();
    check_collisions();
  }

  for(
    unsigned int index = 0;
    index < m_snake.size();
    ++index
  ) {
      
    Cube& cube = m_snake[index];
    if(index == m_head_index) {
      cube.SetColor(snake_head_color);
    } else {
      cube.SetColor(snake_body_color);
    }
    cube.Render();
  }

  for(Cube& wall : m_walls) { 
    wall.SetColor(border_color);
    wall.Render();
  }

  for(Cube& food : m_food) {
    food.SetColor(food_color);
    food.Render();
  }
}

void SnakeScene::controls() {
  if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {
    change_direction(SnakeDirection::UP);
  } 
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
    change_direction(SnakeDirection::DOWN);
  } 
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    change_direction(SnakeDirection::LEFT);
  }
  else if(glfwGetKey(m_engine->glfw_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    change_direction(SnakeDirection::RIGHT);
  }
}

void SnakeScene::init_snake_body() {
  const int shader_id = m_engine->shader().m_ID;
  m_snake.emplace_back(
    shader_id, glm::vec3(0.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    shader_id, glm::vec3(1.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    shader_id, glm::vec3(2.0f, 0.0f, m_z_depth), snake_body_color
  );
  m_snake.emplace_back(
    shader_id, glm::vec3(3.0f, 0.0f, m_z_depth), snake_body_color
  );
}

void SnakeScene::init_walls() {
  const int shader_id = m_engine->shader().m_ID;
  const float scale_factor =
    m_wall_length * 2;

  Cube left(shader_id, glm::vec3(-m_wall_length, 0.0f, m_z_depth));
  left.ScaleY(scale_factor);

  Cube right(shader_id, glm::vec3(m_wall_length, 0.0f, m_z_depth));
  right.ScaleY(scale_factor);

  Cube top(shader_id, glm::vec3(0.0f, m_wall_length, m_z_depth));
  top.ScaleX(scale_factor);

  Cube bottom(shader_id, glm::vec3(0.0f, -m_wall_length, m_z_depth));
  bottom.ScaleX(scale_factor);

  m_walls.push_back(right);
  m_walls.push_back(top);
  m_walls.push_back(bottom);
  m_walls.push_back(left);
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

  Cube cube(m_engine->shader().m_ID, new_cube_pos);
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
  
  const std::string message =
    "Empty Positions: " + std::to_string(empty_positions.size());
  utils::log(message, "SnakeGame");

  return empty_positions;
}

void SnakeScene::spawn_food() {
  std::vector<glm::vec3> positions =
    get_empty_positions();

  if(positions.size() == 0) {
    // Error or end game
    return;
  }

  // Take first one for now
  const glm::vec3 pos = positions[0];
  const int shader_id = m_engine->shader().m_ID;
  m_food.emplace_back(shader_id, pos);
}

void SnakeScene::check_collisions() {

  // Check if eat food, add to snake, and remove
  FoodCollision food_col = has_eaten();
  if(food_col.collide) {
    insert_body_end();
    int index_to_remove = food_col.index;

    m_food.erase(m_food.begin() + index_to_remove);
    spawn_food();

    utils::log("Head Eating", "SnakeGame");
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