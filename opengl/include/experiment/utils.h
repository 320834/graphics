#ifndef EXPERIMENT_UTILS_H_
#define EXPERIMENT_UTILS_H_

#include <iostream>
#include <chrono>

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

inline void log(const std::string& message, const std::string& type = "")
{
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  auto time_print = std::put_time(std::localtime(&t), "%F %T - ");

  const std::string c_type = type.empty() ? ": " : "[" + type + "]: ";
  std::cout << time_print << c_type << message << std::endl;
}

} // namespace utils

#endif //EXPERIMENT_UTILS_H_
