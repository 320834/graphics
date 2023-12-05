#ifndef TRACING_VEC3_H
#define TRACING_VEC3_H

#include <cmath>

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

  Vec3 operator+=(const Vec3 &v);
  Vec3 operator*=(const double t);
  Vec3 operator/=(const double t);

  Vec3 operator+(const Vec3 &right) const;
  Vec3 operator-(const Vec3 &right) const;


  double length() const;
  double length_squared() const;
};

#endif // TRACING_VEC3_H