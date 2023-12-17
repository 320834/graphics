#ifndef TRACING_INTERVAL_H
#define TRACING_INTERVAL_H

#include "utils.h"

class Interval {

public:
  double min, max;

  Interval();
  Interval(double _min, double _max);

  bool contains(double x) const;
  bool surrounds(double x) const;

  double clamp(double x) const;

  static const Interval empty, universe;

};

inline Interval::Interval() : min(+TRACING_INFINITY), max(-TRACING_INFINITY) {}
inline Interval::Interval(double _min, double _max) : min(_min), max(_max) {}

inline bool Interval::contains(double x) const {
  return min <= x && x <= max;
}

inline bool Interval::surrounds(double x) const {
  return min < x && x < max;
}

const static Interval empty(+TRACING_INFINITY, -TRACING_INFINITY);
const static Interval universe(+TRACING_INFINITY, -TRACING_INFINITY);

inline double Interval::clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif // TRACING_INTERVAL_H