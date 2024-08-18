#ifndef EXPERIMENT_CHAR_3D_UTILS_H_
#define EXPERIMENT_CHAR_3D_UTILS_H_

#include "experiment/char_3d_constants.h"
#include "experiment/char_3d.h"
#include <string>

namespace char3d_utils {

inline Character get_character(const char c) {

  if(c == 'A' || c == 'a') {
    return Character::A;
  } else if(c == 'B' || c == 'b') {
    return Character::B;
  } else if(c == 'C' || c == 'c') {
    return Character::C;
  } else if(c == 'D' || c == 'd') {
    return Character::D;
  } else if(c == 'E' || c == 'e') {
    return Character::E;
  } else if(c == 'F' || c == 'f') {
    return Character::F;
  } else if(c == 'G' || c == 'g') {
    return Character::G;
  } else if(c == 'H' || c == 'h') {
    return Character::H;
  } else if(c == 'I' || c == 'i') {
    return Character::I;
  } else if(c == 'J' || c == 'j') {
    return Character::J;
  } else if(c == 'K' || c == 'k') {
    return Character::K;
  } else if(c == 'L' || c == 'l') {
    return Character::L;
  } else if(c == 'M' || c == 'm') {
    return Character::M;
  } else if(c == 'N' || c == 'n') {
    return Character::N;
  } else if(c == 'O' || c == 'o') {
    return Character::O;
  } else if(c == 'P' || c == 'p') {
    return Character::P;
  } else if(c == 'Q' || c == 'q') {
    return Character::Q;
  } else if(c == 'R' || c == 'r') {
    return Character::R;
  } else if(c == 'S' || c == 's') {
    return Character::S;
  } else if(c == 'T' || c == 't') {
    return Character::T;
  } else if(c == 'U' || c == 'u') {
    return Character::U;
  } else if(c == 'V' || c == 'v') {
    return Character::V;
  } else if(c == 'W' || c == 'w') {
    return Character::W;
  } else if(c == 'X' || c == 'x') {
    return Character::X;
  } else if(c == 'Y' || c == 'y') {
    return Character::Y;
  } else if(c == 'Z' || c == 'z') {
    return Character::Z;
  }

  if(c == ' ') {
    return Character::SPACE;
  }

  return Character::NONE;
}

} // namespace char3d_utils

#endif // EXPERIMENT_CHAR_3D_UTILS_H_
