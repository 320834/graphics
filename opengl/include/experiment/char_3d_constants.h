#ifndef EXPERIMENT_CHAR_3D_CONSTANTS_H_
#define EXPERIMENT_CHAR_3D_CONSTANTS_H_

#include "experiment/char_3d.h"

#include <unordered_map>
#include <array>

enum class Character {
  A,B,C,D,E,F,G,H,
  I,J,K,L,M,N,O,P,
  Q,R,S,T,U,V,W,X,
  Y,Z,SPACE,NONE
};

namespace char3d_constants {

const int row_max = 5;
const int col_max = 5;

typedef std::array<std::array<bool, 5>, 5> Array2D;

const Array2D A_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1 
};

const Array2D B_mapping = {
  1, 1, 1, 1, 0,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D C_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1 
};

const Array2D D_mapping = {
  1, 1, 1, 0, 0,
  1, 0, 0, 1, 0,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 0
};

const Array2D E_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1
};

const Array2D F_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0
};

const Array2D G_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 0, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D H_mapping = {
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1
};

const Array2D I_mapping = {
  1, 1, 1, 1, 1,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  1, 1, 1, 1, 1
};

const Array2D J_mapping = {
  0, 0, 0, 0, 1,
  0, 0, 0, 0, 1,
  0, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D K_mapping = {
  1, 0, 0, 0, 1,
  1, 0, 0, 1, 0,
  1, 1, 1, 0, 0,
  1, 0, 0, 1, 0,
  1, 0, 0, 0, 1
};

const Array2D L_mapping = {
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1
};

const Array2D M_mapping = {
  1, 0, 0, 0, 1,
  1, 1, 0, 1, 1,
  1, 0, 1, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1
};

const Array2D N_mapping = {
  1, 0, 0, 0, 1,
  1, 1, 0, 0, 1,
  1, 0, 1, 0, 1,
  1, 0, 0, 1, 1,
  1, 0, 0, 0, 1
};

const Array2D O_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D P_mapping = {
  1, 1, 1, 1, 0,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 0,
  1, 0, 0, 0, 0,
  1, 0, 0, 0, 0
};

const Array2D Q_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 1, 0, 1,
  1, 1, 1, 1, 0
};

const Array2D R_mapping = {
  1, 1, 1, 1, 0,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 0,
  1, 0, 1, 0, 0,
  1, 0, 0, 1, 1
};

const Array2D S_mapping = {
  1, 1, 1, 1, 1,
  1, 0, 0, 0, 0,
  1, 1, 1, 1, 1,
  0, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D T_mapping = {
  1, 1, 1, 1, 1,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0
};

const Array2D U_mapping = {
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 1, 1, 1, 1
};

const Array2D V_mapping = {
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0
};

const Array2D W_mapping = {
  1, 0, 0, 0, 1,
  1, 0, 0, 0, 1,
  1, 0, 1, 0, 1,
  1, 1, 0, 1, 1,
  1, 0, 0, 0, 1
};

const Array2D X_mapping = {
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 1, 0,
  1, 0, 0, 0, 1
};

const Array2D Y_mapping = {
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0
};

const Array2D Z_mapping = {
  1, 1, 1, 1, 1,
  0, 0, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 0, 0,
  1, 1, 1, 1, 1
};

const Array2D none_mapping = {0};

const std::unordered_map<Character, Array2D>
  char_to_mapping = 
{
  {Character::A, A_mapping},
  {Character::B, B_mapping},
  {Character::C, C_mapping},
  {Character::D, D_mapping},
  {Character::E, E_mapping},
  {Character::F, F_mapping},
  {Character::G, G_mapping},
  {Character::H, H_mapping},
  {Character::I, I_mapping},
  {Character::J, J_mapping},
  {Character::K, K_mapping},
  {Character::L, L_mapping},
  {Character::M, M_mapping},
  {Character::N, N_mapping},
  {Character::O, O_mapping},
  {Character::P, P_mapping},
  {Character::Q, Q_mapping},
  {Character::R, R_mapping},
  {Character::S, S_mapping},
  {Character::T, T_mapping},
  {Character::U, U_mapping},
  {Character::V, V_mapping},
  {Character::W, W_mapping},
  {Character::X, X_mapping},
  {Character::Y, Y_mapping},
  {Character::Z, Z_mapping},
  {Character::SPACE, none_mapping},
  {Character::NONE, none_mapping}
};

} // namespace char3d_constants

#endif // EXPERIMENT_CHAR_3D_CONSTANTS_H_
