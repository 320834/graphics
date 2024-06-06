#ifndef TRACING_UTILS_H
#define TRACING_UTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

inline double TRACING_INFINITY = std::numeric_limits<double>::infinity();
inline double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
  return degrees * PI / 180;
}

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

#endif // TRACING_UTILS_H