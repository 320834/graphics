#include "experiment/snake.h"

#include "experiment/engine.h"
#include "experiment/utils.h"

#include <chrono>
#include <vector>

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

const Color border_color = {
  .r = 52,
  .g = 214,
  .b = 235
};

size_t head_index = 0;
std::vector<Cube> g_walls;
SnakeDirection curr_dir = SnakeDirection::LEFT;


void init_game(Engine& engine) { 
  float depth = -15.0f;
  engine.add_cube(
    glm::vec3(0.0f, 0.0f, depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(1.0f, 0.0f, depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(2.0f, 0.0f, depth), snake_body_color
  );

  engine.add_cube(
    glm::vec3(3.0f, 0.0f, depth), snake_body_color
  ); 
  
  // Construct walls
  int shader_id = engine.shader().m_ID;

  Cube left(shader_id, glm::vec3(-5.0f, 0.0f, depth));
  left.ScaleY(10.0f);

  Cube right(shader_id, glm::vec3(5.0f, 0.0f, depth));
  right.ScaleY(10.0f);

  Cube top(shader_id, glm::vec3(0.0f, 5.0f, depth));
  top.ScaleX(10.0f);

  Cube bottom(shader_id, glm::vec3(0.0f, -5.0f, depth));
  bottom.ScaleX(10.0f);

  g_walls.push_back(right);
  g_walls.push_back(top);
  g_walls.push_back(bottom);
  g_walls.push_back(left);

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

void snake_game() {
  Engine engine(
    "Snake Game", 
    "../shaders/experiment/simple.vert",
    "../shaders/experiment/simple.frag",
    640,
    360
  );

  // 1920
  // 1080
  
  // 1280
  // 720

  init_game(engine);

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
        std::cout << "Head is colliding" << std::endl;
      } else {
        std::cout << "Head is not colliding" << std::endl;
      }

      wall.SetColor(border_color);
      wall.Render();
    }
  });
}
