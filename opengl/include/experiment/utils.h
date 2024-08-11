#ifndef EXPERIMENT_UTILS_H_
#define EXPERIMENT_UTILS_H_

#include <iostream>

namespace utils {

inline void print_glm_vec(const glm::vec3 vec) {
  std::cout << 
    "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" <<
    std::endl;
}

inline std::string hash_vec_str(
  const glm::vec3& vec
) {
  int x = static_cast<int>(vec.x);
  int y = static_cast<int>(vec.y);
  int z = static_cast<int>(vec.z);
 
  return std::to_string(x) + std::to_string(y) + std::to_string(z);
}

} // namespace utils

#endif //EXPERIMENT_UTILS_H_
