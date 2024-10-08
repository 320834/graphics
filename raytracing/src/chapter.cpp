#include <iostream>
#include "tracing/chapter.h"
#include "tracing/color.h"
#include "tracing/ray.h"
#include "tracing/hittable.h"
#include "tracing/hittable_list.h"
#include "tracing/sphere.h"
#include "tracing/utils.h"
#include "tracing/interval.h"

namespace tracing {

// void ppm_basics() {
//   const int image_width = 256;
//   const int image_height = 256;

//   std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

//   for(int j = image_height-1; j >= 0; --j) {
//     std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
//     for(int i = 0; i < image_width; ++i) {
//       auto pixel_color = Color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
//       write_color(std::cout, pixel_color);
//     }
//   }

//   std::cerr << "\nDone.\n";
// }

// Color ray_color(const Ray& r, const Hittable& world) {

//   HitRecord rec;
//   if(world.hit(r, Interval(0, TRACING_INFINITY), rec)) {
//     return 0.5 * (rec.normal + Color(1,1,1));
//   }

//   Vec3 unit_direction = unit_vector(r.direction());
//   auto a = 0.5 * (unit_direction.y() + 1.0);
//   return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
// }

// void ray_tracing_setup() {
//   auto aspect_ratio = 16.0 / 9.0;
//   int image_width = 400;

//   // Calculate the image height, and ensure that it's at least 1.
//   int image_height = static_cast<int>(image_width / aspect_ratio);
//   image_height = (image_height < 1) ? 1 : image_height;

//   // World
//   HittableList world;
//   world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
//   world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));

//   // Camera
//   auto focal_length = 1.0;
//   auto viewport_height = 2.0;
//   auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
//   Point3 camera_center = Point3(0,0,0);

//   // Calculate the vectors 
//   Vec3 viewport_u = Vec3(viewport_width, 0, 0);
//   Vec3 viewport_v = Vec3(0, -viewport_height, 0);

//   Vec3 pixel_delta_u = viewport_u / image_width;
//   Vec3 pixel_delta_v = viewport_v / image_height;

//   Vec3 viewport_upper_left = 
//     camera_center - Vec3(0,0, focal_length) - viewport_u/2 - viewport_v/2;
//   Vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

//   // Render

//   std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

//   for (int j = 0; j < image_height; ++j) {
//       std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
//       for (int i = 0; i < image_width; ++i) {
//         auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
//         auto ray_direction = pixel_center - camera_center;
//         Ray r(camera_center, ray_direction);

//         Color pixel_color = ray_color(r, world);
//         write_color(std::cout, pixel_color);
//       }
//   }

//   std::clog << "\rDone.                 \n";

// }

} // namespace tracing