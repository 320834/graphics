#ifndef EXPERIMENT_TEXT_H_
#define EXPERIMENT_TEXT_H_

#include "experiment/cube.h"

class Letter {
public:
  Letter(
    const unsigned int shader_id,
    float size = 1.0f
  );
  void render();
  void construct_letter(bool mapping[5][4]);
  
protected:
  const unsigned int m_shader_id;
  float m_size;
  std::vector<Cube> m_blocks;
};

class Letter_A : public Letter {
public:
  Letter_A(
    const unsigned int shader_id,
    float size = 1.0f
  );
};

#endif // EXPERIMENT_TEXT_H_
