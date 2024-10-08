#ifndef TRACING_SPHERE_H
#define TRACING_SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
  Sphere(Point3 _center, double _radius);

  bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

private:
  Point3 center;
  double radius;
};

inline Sphere::Sphere(Point3 _center, double _radius) : center(_center), radius(_radius) {};

inline bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
  Vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius*radius;

  auto discriminant = half_b*half_b - a*c;
  if (discriminant < 0) return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (!ray_t.surrounds(root)) {
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root))
          return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  Vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);

  return true;
}


#endif // TRACING_SPHERE_H