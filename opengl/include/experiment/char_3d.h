#ifndef EXPERIMENT_CHAR_3D_H_
#define EXPERIMENT_CHAR_3D_H_

#include "experiment/cube.h"
#include "experiment/char_3d_constants.h"
#include "experiment/engine.h"

#include <glm/glm.hpp>

#include <array>

const Color default_letter_color = {
  .r = 255,
  .g = 255,
  .b = 255
};

class Char3D {
public:
  Char3D(
    const std::shared_ptr<Engine>& engine,
    const Character ch,
    float size = 1.0f
  );
  void render();

  void transform(const glm::vec3& pos);
  
private:
  void build_letter(); 
  void construct_letter_blocks(const std::array<std::array<bool, 5>, 5> mapping);

  const Character m_ch;
  std::shared_ptr<Engine> m_engine;
  float m_size;
  std::vector<Cube> m_blocks;
};

class PhraseBuilder {
public:
  PhraseBuilder(
    const std::shared_ptr<Engine>& engine,
    const std::string& text,
    const float size = 1.0f
  );

  void render();
  void transform(const glm::vec3 pos);
  std::string text();
  glm::vec3 position();
private:
  
  std::vector<Character> extract_chars();
  
  glm::mat4 m_transformation;
  const std::shared_ptr<Engine> m_engine;
  std::string m_text;
  float m_size;
  std::vector<Char3D> m_chars;
};

#endif // EXPERIMENT_CHAR_3D_H_
