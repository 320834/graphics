#ifndef EXPERIMENT_CHAR_3D_CONSTANTS_H_
#define EXPERIMENT_CHAR_3D_CONSTANTS_H_

#include <unordered_map>

enum class Character;

namespace char3d_constants {

const int row_max = 5;
const int col_max = 5;

const bool A_mapping[5][5] = {
  { 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 1 }
};

const bool B_mapping[5][5] = {
  { 1, 1, 1, 1, 0 },
  { 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1 }
};

const bool C_mapping[5][5] = {
  { 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1 } 
};

const bool D_mapping[5][5] = {
  { 1, 1, 1, 0, 0 },
  { 1, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 0 }
};

const bool none_mapping[5][5] = {0};

const std::unordered_map<Character, const bool[5][5]>
  char_to_mapping = 
{
  std::make_pair(Character::A, A_mapping)
  // {Character::B, B_mapping},
  // {Character::C, C_mapping},
  // {Character::D, D_mapping}
};

} // namespace char3d_constants

#endif // EXPERIMENT_CHAR_3D_CONSTANTS_H_
