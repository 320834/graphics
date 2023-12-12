#ifndef TRACING_UTILS_H
#define TRACING_UTILS_H

#include <cmath>
#include <limits>
#include <memory>

inline double TRACING_INFINITY = std::numeric_limits<double>::infinity();
inline double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
  return degrees * PI / 180;
}

#endif // TRACING_UTILS_H