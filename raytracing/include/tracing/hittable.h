#ifndef TRACING_HITTABLE_H
#define TRACING_HITTABLE_H

#include "ray.h"
#include "interval.h"

class HitRecord {
public:
  Point3 p;
  Vec3 normal;
  double t;
  bool front_face;

  void set_face_normal(const Ray& r, const Vec3& outward_normal);
};

inline void HitRecord::set_face_normal(const Ray& r, const Vec3& outward_normal) {
  if(dot(r.direction(), outward_normal) >= 0) {
    normal = -outward_normal; 
    front_face = false;
  } else {
    normal = outward_normal;
    front_face = true;
  }
}

class Hittable {
public:
  virtual ~Hittable() = default;
  virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const = 0;
};

#endif // TRACING_HITTABLE_H