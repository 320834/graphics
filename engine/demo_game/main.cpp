#include <iostream>
#include "opengl_wrapper.h"
#include "engine.h"
#include "shapes/cube.h"
#include "shapes/square.h"

#include "generic_scene.h"

int main() {

  std::shared_ptr<Engine<OpenGLWrapper>> engine =
    std::make_shared<Engine<OpenGLWrapper>>(
      "Snake Game", 
      "../shaders/simple.vert",
      "../shaders/simple.frag",
      // 1920,
      // 1080
      960,
      540
    );

  Cube::init_vertex_buffers();
  Square::init_vertex_buffers();

  std::shared_ptr<GenericScene> gen_scene =
    std::make_shared<GenericScene>(
      "main"
    );

  engine->scene_manager().add_scene(gen_scene);
  engine->scene_manager().set_current_scene("main");

  engine->loop();
}
