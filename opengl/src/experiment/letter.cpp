#include "experiment/letter.h"

Letter::Letter(
  const unsigned int shader_id,
  float size
)
  : m_shader_id{shader_id},
    m_size{size}
{}

void Letter::construct_letter(
  bool mapping[5][4]
)
{
  // Placeholder
  const unsigned int shader_id = 1;
  
  // for(size_t col = 4; col < )
  
}

void Letter::render() {
  for(Cube& block : m_blocks) {
    block.Render();
  }
}

Letter_A::Letter_A(
  const unsigned int shader_id,
  float size
) 
  : Letter(shader_id, size)
{
}
