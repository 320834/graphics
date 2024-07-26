#include "experiment/snake.h"

#include "experiment/engine.h"
#include "experiment/utils.h"

#include <chrono>
#include <vector>

size_t head_index = 0;
SnakeDirection curr_dir = SnakeDirection::LEFT;

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
    head_index == 0 ? 
    engine.m_cubes.size() - 1 :
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
    // head_index == 0 ?
    // engine.m_cubes.size() - 1 :
    // head_index - 1;

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
    1920,
    1080
  );

  engine.add_cube(
    glm::vec3(0.0f, 0.0f, -5.0f)
  );

  engine.add_cube(
    glm::vec3(1.0f, 0.0f, -5.0f)
    //"awesomeface.png"
  );

  engine.add_cube(
    glm::vec3(2.0f, 0.0f, -5.0f)
    //"container.jpg"
  );

  engine.add_cube(
    glm::vec3(3.0f, 0.0f, -5.0f)
  );

  std::chrono::time_point last_record =
    std::chrono::system_clock::now();

  std::chrono::time_point last_a_record =
    std::chrono::system_clock::now();

  engine.loop([&last_record, &last_a_record](Engine& engine) {

    std::chrono::time_point now =
      std::chrono::system_clock::now();
    std::chrono::time_point now_a =
      std::chrono::system_clock::now();

    std::chrono::milliseconds duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - last_record);

    auto duration_a =
      std::chrono::duration_cast<std::chrono::milliseconds>(now_a - last_a_record);

    if(
        glfwGetKey(engine.glfw_window(), GLFW_KEY_P) == GLFW_PRESS &&
        duration_a.count() > 1000
      )
    {
      last_a_record = std::chrono::system_clock::now();
      insert_end(engine);
    } else {
      if(duration.count() >= 500) {
        last_record = std::chrono::system_clock::now();
        next_tick(engine);
      }
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
    
       
    for(auto cube : engine.m_cubes) {
      cube.Render();
    }

    // Need to figure out list of cubes
    // Move in negative position, alert position
    // of i - 1
    // [ 0, 1, 2, 3 ]

    // If we want to insert, insert at head_index - 1
    // Somehow get direction of second to last and last

  });
}
