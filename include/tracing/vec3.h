#ifndef TRACING_VEC3_H
#define TRACING_VEC3_H

#include <cmath>
#include <iostream>
#include "utils.h"

class Vec3 {
public:
  Vec3();
  Vec3(double e0, double d1, double e2);

  double x() const;
  double y() const;
  double z() const;

  Vec3 operator-() const;
  double operator[](int i) const;
  double& operator[](int i);

  Vec3& operator+=(const Vec3 &v);
  Vec3& operator*=(const double t);
  Vec3& operator/=(const double t);

  double length() const;
  double length_squared() const;

  static Vec3 random();
  static Vec3 random(double min, double max);

  Vec3 random_in_unit_sphere();
  Vec3 random_unit_vector();
  Vec3 random_on_hemisphere(const Vec3& normal);

  friend std::ostream& operator<<(std::ostream& out, const Vec3 &v);
  friend Vec3 operator+(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator-(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator*(const Vec3 &u, const Vec3 &v);
  friend Vec3 operator*(double t, const Vec3 &v);
  friend Vec3 operator*(const Vec3 &v, double t);
  friend Vec3 operator/(const Vec3 v, double t);

  friend double dot(const Vec3 &u, const Vec3 &v);
  friend Vec3 cross(const Vec3 &u, const Vec3 &v);
  friend Vec3 unit_vector(Vec3 v);

protected:
  double e[3];
};

inline Vec3::Vec3() : e{0,0,0} 
{}

inline Vec3::Vec3(double e0, double e1, double e2) : e{e0,e1,e2} 
{}

inline double Vec3::x() const { return e[0]; }
inline double Vec3::y() const { return e[1]; }
inline double Vec3::z() const { return e[2]; }

inline Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

inline double Vec3::operator[](int i) const { return e[i]; }
inline double& Vec3::operator[](int i) { return e[i]; }

inline Vec3& Vec3::operator+=(const Vec3 &v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  
  return *this;
}

inline Vec3& Vec3::operator*=(const double t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  
  return *this;
}

inline Vec3& Vec3::operator/=(double t) {
  return *this *= 1/t;
}

inline double Vec3::length() const {
  return sqrt(length_squared());
}

inline double Vec3::length_squared() const {
  return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

inline std::ostream& operator<<(std::ostream& out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
  return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
  return t * v; // Use previous * operator overload
}

inline Vec3 operator/(Vec3 v, double t) {
  return (1/t) * v;
}

// Probably the most important functions
inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unit_vector(Vec3 v) {
  return v/v.length();
}

inline Vec3 Vec3::random() {
  return Vec3(random_double(), random_double(), random_double());
}

inline Vec3 Vec3::random(double min, double max) {
  return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

inline Vec3 Vec3::random_in_unit_sphere() {
  while (true) {
    auto p = Vec3::random(-1,1);
    if (p.length_squared() < 1)
      return p;
  }
}

inline Vec3 Vec3::random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

inline Vec3 Vec3::random_on_hemisphere(const Vec3& normal) {
  Vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return on_unit_sphere;
  else
    return -on_unit_sphere;
}

using Point3 = Vec3;

#endif // TRACING_VEC3_H