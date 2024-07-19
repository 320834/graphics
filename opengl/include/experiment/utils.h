#ifndef EXPERIMENT_UTILS_H_
#define EXPERIMENT_UTILS_H_

#include <iostream>

namespace utils {

inline void print_glm_vec(const glm::vec3 vec) {
  std::cout << 
    "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" <<
    std::endl;
}

} // namespace utils

#endif //EXPERIMENT_UTILS_H_
