#include <iostream>
#include "opengl_wrapper.h"
#include "engine.h"
#include "shapes/cube.h"

#include "generic_scene.h"
#include "shapes/dynamic.h"

int main() {

  ShaderFiles shaders = {
    .simple_vertex_shader = "../demo_game/shaders/simple.vert",
    .simple_fragment_shader = "../demo_game/shaders/simple.frag",
    .assimp_vertex_shader = "../demo_game/shaders/assimp.vert",
    .assimp_fragment_shader = "../demo_game/shaders/assimp.frag"
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

  DynamicShapeManager::load_shape("backpack", "../demo_game/models/backpack/backpack.obj");
  Cube::init_vertex_buffers();

  std::shared_ptr<GenericScene> gen_scene =
    std::make_shared<GenericScene>(
      engine,
      "main"
    );

  engine->scene_manager().add_scene(gen_scene);
  engine->scene_manager().set_current_scene("main");

  engine->loop();
}
