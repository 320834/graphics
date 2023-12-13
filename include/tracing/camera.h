#ifndef TRACING_CAMERA_H
#define TRACING_CAMERA_H

#include "hittable.h"
#include "color.h"
#include "interval.h"
#include "ray.h"

class Camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  void render(const Hittable& world);
private:
  int    image_height;   // Rendered image height
  Point3 center;         // Camera center
  Point3 pixel00_loc;    // Location of pixel 0, 0
  Vec3   pixel_delta_u;  // Offset to pixel to the right
  Vec3   pixel_delta_v;  // Offset to pixel below

  void initialize();
  Color ray_color(const Ray& ray, const Hittable& world) const;


};

inline void Camera::render(const Hittable& world) {

  initialize();

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = 0; j < image_height; ++j) {
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto ray_direction = pixel_center - center;
        Ray r(center, ray_direction);

        Color pixel_color = ray_color(r, world);
        write_color(std::cout, pixel_color);
      }
  }

  std::clog << "\rDone.                 \n";

}

inline void Camera::initialize() {

  // Calculate the image height, and ensure that it's at least 1.
  image_height = static_cast<int>(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;

  // World
  HittableList world;
  world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
  world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));

  center = Point3(0,0,0);

  // Camera
  auto focal_length = 1.0;
  auto viewport_height = 2.0;
  auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

  // Calculate the vectors 
  Vec3 viewport_u = Vec3(viewport_width, 0, 0);
  Vec3 viewport_v = Vec3(0, -viewport_height, 0);

  pixel_delta_u = viewport_u / image_width;
  pixel_delta_v = viewport_v / image_height;

  Vec3 viewport_upper_left = 
    center - Vec3(0,0, focal_length) - viewport_u/2 - viewport_v/2;
  pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

inline Color Camera::ray_color(const Ray& ray, const Hittable& world) const {
  HitRecord rec;

  if(world.hit(ray, Interval(0, TRACING_INFINITY), rec)) {
    return 0.5 * (rec.normal + Color(1,1,1));
  }

  Vec3 unit_direction = unit_vector(ray.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
};

#endif // TRACING_CAMERA_H