#ifndef EXPERIMENT_CHAR_3D_H_
#define EXPERIMENT_CHAR_3D_H_

#include "experiment/cube.h"
#include "experiment/char_3d_constants.h"

#include <array>

const Color default_letter_color = {
  .r = 255,
  .g = 255,
  .b = 255
};

class Char3D {
public:
  Char3D(
    const unsigned int shader_id,
    const Character ch,
    float size = 1.0f
  );
  void render();

  void transform(const glm::vec3& pos);
  
private:
  void build_letter(); 
  void construct_letter_blocks(const std::array<std::array<bool, 5>, 5> mapping);

  const Character m_ch;
  const unsigned int m_shader_id;
  float m_size;
  std::vector<Cube> m_blocks;
};

class PhraseBuilder {
public:
  PhraseBuilder(
    const unsigned int shader_id,
    const std::string& text,
    const float size = 1.0f
  );

  void render();
  void transform(const glm::vec3& pos);
private:
  
  std::vector<Character> extract_chars();
  void build_phrase();

  const unsigned int m_shader_id;
  std::string m_text;
  float m_size;
  std::vector<Char3D> m_chars;
};

#endif // EXPERIMENT_CHAR_3D_H_
