/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ceres/exceres.h"
#include "3rdparty/opencv/exopencv.h"

#include <type_traits>

namespace earthxpert {
/**
 *  \brief ceres::Jet<> compatible arithmic operators on Point3d
 *
 *  \param Ty acceptable types: double, ceres::Jet<>
 */
template <typename Ty, std::enable_if_t<!std::is_same_v<Point3d, Ty>, int> = 0> 
struct Point3 {
  Ty x{};
  Ty y{};
  Ty z{};

  Point3(const Ty& x, const Ty& y, const Ty& z)
    : x(x), y(y), z(z) {}

  Point3(const Point3d& rhs)
    : x(Ty(rhs.x)), y(Ty(rhs.y)), z(Ty(rhs.z)) {}

  Point3<Ty>& operator=(const Point3d& p) {
    x = Ty(p.x);
    y = Ty(p.y);
    z = Ty(p.z);
    return *this;
  }
};

/**
 *  \brief arithmetic operator
 */
template<typename Ty> inline
Point3<Ty> operator+(const Point3<Ty>& p, const Ty& s) {
  return { p.x + s, p.y + s, p.z + s };
}

template<typename Ty> inline
Point3<Ty> operator+(const Point3<Ty>& p1, const Point3<Ty>& p2) {
  return { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}

template<typename Ty> inline
Point3<Ty> operator+(const Point3d& p1, const Point3<Ty>& p2) {
  return Point3<Ty>{ Ty(p1.x) + p2.x, Ty(p1.y) + p2.y, Ty(p1.z) + p2.z };
}

template<typename Ty> inline
Point3<Ty> operator-(const Point3<Ty>& p1, const Point3<Ty>& p2) {
  return { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
}

template<typename Ty> inline 
Point3<Ty> operator*(const Point3<Ty>& p, const Ty& s) {
  return { p.x * s, p.y * s, p.z * s };
}

/**
 *  \brief math functions
 */
// norm of 3d vector
template <typename Ty> inline
Ty norm(const Point3<Ty>& p) {
  return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

// distance from 3d point to an infinit ray defined by two 3d points.
template <typename Ty> inline
Ty distance(const Point3<Ty>& p, const Point3d* seg) {
  return {};
}
}
