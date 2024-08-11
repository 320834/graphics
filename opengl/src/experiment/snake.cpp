#include "experiment/snake.h"

#include "experiment/engine.h"
#include "experiment/utils.h"

#include <chrono>
#include <vector>
#include <unordered_set>

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
  .r = 52,
  .g = 214,
  .b = 235
};

size_t head_index = 0;
std::chrono::time_point next_tick_last = std::chrono::system_clock::now();
std::chrono::time_point append_last = std::chrono::system_clock::now();
int g_max_food = 1;
std::vector<Cube> g_food;
SnakeDirection curr_dir = SnakeDirection::LEFT;

// Wall attributes
std::vector<Cube> g_walls;
float g_wall_length = 6.0f;
float g_z_depth = -15.0f;

void init_game(Engine& engine) { 
  engine.add_cube(
    glm::vec3(0.0f, 0.0f, g_z_depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(1.0f, 0.0f, g_z_depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(2.0f, 0.0f, g_z_depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(3.0f, 0.0f, g_z_depth), snake_body_color
  ); 
  
  // Construct walls
  int shader_id = engine.shader().m_ID;

  const float scale_factor =
    g_wall_length * 2;

  Cube left(shader_id, glm::vec3(-g_wall_length, 0.0f, g_z_depth));
  left.ScaleY(scale_factor);

  Cube right(shader_id, glm::vec3(g_wall_length, 0.0f, g_z_depth));
  right.ScaleY(scale_factor);

  Cube top(shader_id, glm::vec3(0.0f, g_wall_length, g_z_depth));
  top.ScaleX(scale_factor);

  Cube bottom(shader_id, glm::vec3(0.0f, -g_wall_length, g_z_depth));
  bottom.ScaleX(scale_factor);

  g_walls.push_back(right);
  g_walls.push_back(top);
  g_walls.push_back(bottom);
  g_walls.push_back(left);

}

FoodCollision has_eaten(Engine& engine) {
  if(engine.m_cubes.size() == 0) {
    return {
      .collide = false,
      .index = -1
    };
  }

  Cube& head = engine.m_cubes[head_index];

  for(int index = 0; index < g_food.size(); ++index) {
    Cube::Collision col = g_food[index].IsColliding(head); 
    // std::cout << "In: " << std::to_string(index) << std::endl;
 
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

void change_direction(SnakeDirection new_dir)
{
  if(
    (curr_dir == SnakeDirection::LEFT &&
    new_dir == SnakeDirection::RIGHT) ||
    (curr_dir == SnakeDirection::UP &&
    new_dir == SnakeDirection::DOWN) ||
    (curr_dir == SnakeDirection::DOWN &&
    new_dir == SnakeDirection::UP) ||
    (curr_dir == SnakeDirection::RIGHT &&
    new_dir == SnakeDirection::LEFT)
  ) {
    return;
  }

  curr_dir = new_dir;
}

void next_tick(Engine& engine) {

  size_t tail_index = 
    head_index == 0 ? engine.m_cubes.size() - 1 :
    head_index - 1;

  if(head_index >= engine.m_cubes.size() ||
     tail_index >= engine.m_cubes.size())
  {
    return;
  }

  Cube& head_cube = engine.m_cubes[head_index];
  Cube& tail_cube = engine.m_cubes[tail_index];

  tail_cube.SetPosition(head_cube.GetPosition());

  // Determine direction here
  switch(curr_dir) {
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
  if(head_index == 0) {
    head_index = engine.m_cubes.size() - 1;
  } else {
    head_index -= 1;
  }
}

glm::vec3 get_append_pos(Engine& engine) {
  size_t tail_index = 
     head_index == 0 ? 
     engine.m_cubes.size() - 1 :
     head_index - 1;

  size_t second_last =
    tail_index == 0 ?
    engine.m_cubes.size() - 1 :
    tail_index - 1;

  Cube& tail_cube = engine.m_cubes[tail_index];
  Cube& second_last_cube = engine.m_cubes[second_last];
  
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

void insert_end(Engine& engine) {

  size_t insert_index = head_index;

  glm::vec3 new_cube_pos = get_append_pos(engine);

  auto it = engine.m_cubes.begin() + insert_index;

  head_index = head_index + 1;

  Cube cube(engine.shader().m_ID,new_cube_pos);
  engine.m_cubes.insert(it, cube);
}

// Generate a list of empty positions 
// for food generation
std::vector<glm::vec3> get_empty_positions(Engine& engine) {
  
  const int hash_table_size =
    100 *
    static_cast<int>(g_wall_length) *
    static_cast<int>(g_wall_length);

  std::unordered_set<std::string> filled;
  for(Cube& snake : engine.m_cubes) {
    const glm::vec3 pos = snake.GetPosition();
    std::string hash =
     utils::hash_vec_str(pos);
    

    
    filled.emplace(hash);
  }

  for(Cube& food : g_food) {
    const glm::vec3 pos = food.GetPosition();
    std::string hash =
      utils::hash_vec_str(pos);
    filled.emplace(hash);
  }

  std::vector<glm::vec3> empty_positions;
  for(int x = -g_wall_length + 1; x < g_wall_length; ++x)
  {
    for(int y = g_wall_length - 1; y > -g_wall_length; --y) 
    {
      const glm::vec3 potential(x, y, g_z_depth);

      std::string hash =
        utils::hash_vec_str(potential);

      if(filled.count(hash) == 0) {
         empty_positions.push_back(potential);
      }
    }
  }

  std::cout << "Empty Positions: " << std::to_string(empty_positions.size()) << std::endl;

  return empty_positions;
}

void spawn_food(Engine& engine) {
  std::vector<glm::vec3> positions =
    get_empty_positions(engine);

  if(positions.size() == 0) {
    // Error or end game
    return;
  }

  // Take first one for now
  const glm::vec3 pos = positions[0];
  const int shader_id = engine.shader().m_ID;
  g_food.emplace_back(shader_id, pos);

}

void snake_game() {
  Engine engine(
    "Snake Game", 
    "../shaders/experiment/simple.vert",
    "../shaders/experiment/simple.frag",
    640,
    460
  );

  // 1920
  // 1080
  
  // 1280
  // 720

  init_game(engine);

  spawn_food(engine);

  std::chrono::time_point next_tick_last = std::chrono::system_clock::now();
  std::chrono::time_point append_last = std::chrono::system_clock::now();

  
  engine.loop([&next_tick_last, &append_last](Engine& engine) {

    std::chrono::time_point now =
      std::chrono::system_clock::now();

    auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - next_tick_last);

    auto duration_a =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - append_last);

    if(
        glfwGetKey(engine.glfw_window(), GLFW_KEY_P) == GLFW_PRESS &&
        duration_a.count() > 1000
      )
    {
      append_last = std::chrono::system_clock::now();
      insert_end(engine);
    }
    
    if(duration.count() >= 500) {
      next_tick_last = std::chrono::system_clock::now();
      next_tick(engine);
    }
        
    if(glfwGetKey(engine.glfw_window(), GLFW_KEY_UP) == GLFW_PRESS) {
      change_direction(SnakeDirection::UP);
    } 
    else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      change_direction(SnakeDirection::DOWN);
    } 
    else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      change_direction(SnakeDirection::LEFT);
    }
    else if(glfwGetKey(engine.glfw_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      change_direction(SnakeDirection::RIGHT);
    }

    // Check if eat food, add to snake, and remove
    FoodCollision food_col = has_eaten(engine);
    if(food_col.collide) {
      
      insert_end(engine);
      int index_to_remove = food_col.index;

      g_food.erase(g_food.begin() + index_to_remove);

      spawn_food(engine);
    }

    // Check if head is touching walls
    Cube& head = engine.m_cubes[head_index];


    for(
      unsigned int index = 0;
      index < engine.m_cubes.size();
      ++index
    ) {
      
      Cube& cube = engine.m_cubes[index];
      if(index == head_index) {
        cube.SetColor(snake_head_color);
      } else {
        cube.SetColor(snake_body_color);
      }

      cube.Render();
    }

    for(Cube& wall : g_walls) {
      
      Cube::Collision col = wall.IsColliding(head);
      if(col.collide && col.points >= 8) {
        // std::cout << "Head is colliding" << std::endl;
      } else {
        // std::cout << "Head is not colliding" << std::endl;
      }

      wall.SetColor(border_color);
      wall.Render();
    }

    for(Cube& food : g_food) {
      food.SetColor(food_color);
      food.Render();
    }
  });
}
