#ifndef TRACING_COLOR_H
#define TRACING_COLOR_H

#include "vec3.h"
#include <iostream>

using Color = Vec3;

inline void write_color(std::ostream& out, Color pixel_color) {
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif