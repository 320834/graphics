#ifndef TRACING_HITTABLE_LIST_H
#define TRACING_HITTABLE_LIST_H

#include "hittable.h"

#include <vector>
#include <memory>

class HittableList : public Hittable {

public:
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList();
  HittableList(std::shared_ptr<Hittable> object);

  void clear();

  void add(std::shared_ptr<Hittable> object);
  bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

};

inline HittableList::HittableList() {}

inline HittableList::HittableList(std::shared_ptr<Hittable> object) {
  add(object);
}

inline void HittableList::clear() {
  objects.clear();
}

inline void HittableList::add(std::shared_ptr<Hittable> object) {
  objects.push_back(object);
}

inline bool HittableList::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
  HitRecord temp_rec;
  bool hit_anything = false;
  auto closest_so_far = ray_t.max;

  for (const auto& object : objects) {
    if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

#endif // TRACING_HITTABLE_LIST_H