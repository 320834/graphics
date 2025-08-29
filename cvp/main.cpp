#include <iostream>

#include <iostream>
#include "opengl_wrapper.h"
#include "engine.h"
#include "shapes/cube.h"

#include "main_scene.h"
#include "shapes/dynamic.h"

int main() {
  ShaderFiles shaders = {
    .simple_vertex_shader = "../shaders/simple.vert",
    .simple_fragment_shader = "../shaders/simple.frag",
    .assimp_vertex_shader = "../shaders/assimp.vert",
    .assimp_fragment_shader = "../shaders/assimp.frag"
  };

  std::shared_ptr<Engine<OpenGLWrapper>> engine =
    std::make_shared<Engine<OpenGLWrapper>>(
      "Snake Game", 
      1920,
      1080,
      // 720,
      // 360,
      shaders
    );

  DynamicShapeManager::load_shape(
    "backpack",
    "../assets/backpack/backpack.obj"
  );
  Cube::init_vertex_buffers();

  std::shared_ptr<MainScene> gen_scene =
    std::make_shared<MainScene>(
      engine,
      "main"
    );

  engine->scene_manager().add_scene(gen_scene);
  engine->scene_manager().set_current_scene("main");

  engine->loop();
}