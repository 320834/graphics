#ifndef TRACING_RAY_H
#define TRACING_RAY_H

#include "vec3.h"

class Ray {
public:
  Ray();
  Ray(const Point3& origin, const Vec3& direction);

  Point3 origin() const;
  Vec3 direction() const;

  Point3 at(double t) const;

private:
  Point3 orig;
  Vec3 dir;
};

inline Ray::Ray() {};
inline Ray::Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

inline Point3 Ray::origin() const { return orig; }
inline Vec3 Ray::direction() const { return dir; }

inline Point3 Ray::at(double t) const {
  return orig + (t * dir);
}

#endif // TRACING_RAY_H