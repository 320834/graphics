#ifndef TRACING_HITTABLE_H
#define TRACING_HITTABLE_H

#include "ray.h"

class HitRecord {
public:
  Point3 p;
  Vec3 normal;
  double t;
};

class Hittable {
public:
  virtual ~Hittable() = default;
  virtual bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

#endif // TRACING_HITTABLE_H