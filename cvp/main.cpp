#include <iostream>
#include <string>
#include <memory>

#include "opengl_wrapper.h"
#include "engine.h"
#include "shapes/cube.h"

#include "naive_camera_scene.h"
#include "pbo_test.h"
#include "shapes/dynamic.h"

namespace {
  enum class SceneType { Naive, Pbo };

  void print_usage(const char* prog) {
    std::cout
      << "Usage: " << prog << " [--type <naive|pbo>]\n"
      << "  -t, --type   Select scene type (default: pbo)\n";
  }

  SceneType parse_type(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      if ((arg == "-t" || arg == "--type") && i + 1 < argc) {
        std::string val = argv[i + 1];
        if (val == "naive") {
          return SceneType::Naive; 
        }
        if (val == "pbo") {
          return SceneType::Pbo;
        }
        std::cerr << "Unknown type: " << val << "\n";
        print_usage(argv[0]);
        std::exit(EXIT_FAILURE);
      }
      if (arg == "-h" || arg == "--help") {
        print_usage(argv[0]);
        std::exit(EXIT_SUCCESS);
      }
    }
    return SceneType::Pbo; // default
  }
}

int main(int argc, char** argv) {
  const SceneType type = parse_type(argc, argv);

  ShaderFiles shaders = {
    .simple_vertex_shader  = "../shaders/simple.vert",
    .simple_fragment_shader= "../shaders/simple.frag",
    .assimp_vertex_shader  = "../shaders/assimp.vert",
    .assimp_fragment_shader= "../shaders/assimp.frag"
  };

  auto engine = std::make_shared<Engine<OpenGLWrapper>>(
    "PBO Test",
    720,
    360,
    shaders
  );

  Cube::init_vertex_buffers();

  if (type == SceneType::Naive) {
    auto gen_scene = std::make_shared<NaiveCameraScene>(engine, "camera");
    engine->scene_manager().add_scene(gen_scene);
    engine->scene_manager().set_current_scene("camera");
  } else {
    auto gen_scene = std::make_shared<PboTestScene>(engine, "pbo_test");
    engine->scene_manager().add_scene(gen_scene);
    engine->scene_manager().set_current_scene("pbo_test");
  }

  engine->loop();
  return 0;
}
