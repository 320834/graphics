#include "experiment/char_3d.h"
#include "experiment/char_3d_constants.h"
#include "experiment/char_3d_utils.h"

#include <array>

Char3D::Char3D(
  const unsigned int shader_id,
  const Character ch,
  float size
)
  : m_shader_id{shader_id},
    m_size{size},
    m_ch{ch}
{
  build_letter(); 
}

void Char3D::render() {
  for(Cube& block : m_blocks) {
    block.Render();
  }
}

void Char3D::transform(const glm::vec3& pos) {
  for(Cube& block : m_blocks) {
    block.Transform(pos);
  }
}

void Char3D::build_letter() {
  auto search =
    char3d_constants::char_to_mapping.find(m_ch);
  
  if(search != char3d_constants::char_to_mapping.end()) {
    construct_letter_blocks(search->second);
  } else {
    construct_letter_blocks(char3d_constants::none_mapping);
  }
}

void Char3D::construct_letter_blocks(
  const std::array<std::array<bool, 5>, 5> mapping
)
{
  // Placeholder
  const unsigned int shader_id = 1;
  const int row_max = char3d_constants::row_max;
  const int col_max = char3d_constants::col_max;

  for(int row = row_max - 1; row >= 0; --row) {
    for(int col = 0; col < col_max; ++col) {
      if(mapping[row][col] == 0) {
        continue;
      }

      const int row_pos = (row_max - 1) - row;
      const int col_pos = col;

      const float x = col_pos * m_size;
      const float y = row_pos * m_size;
      glm::vec3 pos(x,y,0);

      Cube block(m_shader_id, pos, default_letter_color);
      block.Scale(m_size);

      m_blocks.push_back(block);
      
    }
  }
  
}

PhraseBuilder::PhraseBuilder(
  const unsigned int shader_id,
  const std::string& text,
  const float size
)
  : m_shader_id{shader_id},
    m_text{text},
    m_size{size}
{
  // Extract chars
  // Do Spacing
  std::vector<Character> chars =
    extract_chars();

  for(int i = 0; i < chars.size(); ++i) {
    const Character ch = chars[i];
    const float spacing = 2;
    float x = i * m_size * (char3d_constants::col_max + spacing);

    Char3D block(m_shader_id, ch, m_size);
    block.transform(glm::vec3(x, 0, 0));

    m_chars.push_back(block);
  }

}

void PhraseBuilder::render() {
  for(Char3D& chars : m_chars) {
    chars.render();
  }
}

void PhraseBuilder::transform(const glm::vec3& pos) {
  for(Char3D& chars : m_chars) {
    chars.transform(pos);
  }
}

std::vector<Character> PhraseBuilder::extract_chars() {
  
  std::vector<Character> chars;

  for(const char c : m_text) {
    const Character ch =
      char3d_utils::get_character(c);
    
    chars.push_back(ch);
  }

  return chars;

}

void PhraseBuilder::build_phrase() {

}
