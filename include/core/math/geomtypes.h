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

  Point3<Ty>& operator+=(const Ty& s) {
    x += s;
    y += s;
    z += s;
    return *this;
  }

  Point3<Ty>& operator+=(const Point3<Ty>& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Point3<Ty>& operator+=(const Point3d& rhs) {
    x += Ty(rhs.x);
    y += Ty(rhs.y);
    z += Ty(rhs.z);
    return *this;
  }

  Point3<Ty>& operator-=(const Ty& s) {
    x -= s;
    y -= s;
    z -= s;
    return *this;
  }

  Point3<Ty>& operator-=(const Point3d& rhs) {
    x -= Ty(rhs.x);
    y -= Ty(rhs.y);
    z -= Ty(rhs.z);
    return *this;
  }

  Point3<Ty>& operator*=(const Ty& s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  Point3<Ty>& operator/=(const Ty& s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }
};

/**
 *  \brief arithmetic operator
 */
template<typename Ty> inline
Point3<Ty> operator+(const Point3<Ty>& lhs, const Ty& s) {
  return { lhs.x + s, lhs.y + s, lhs.z + s };
}

template<typename Ty> inline
Point3<Ty> operator+(const Point3<Ty>& lhs, const Point3<Ty>& rhs) {
  return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template<typename Ty> inline
Point3<Ty> operator+(const Point3d& lhs, const Point3<Ty>& rhs) {
  return Point3<Ty>{ Ty(lhs.x) + rhs.x, Ty(lhs.y) + rhs.y, Ty(lhs.z) + rhs.z };
}

template<typename Ty> inline
Point3<Ty> operator-(const Point3<Ty>& lhs, const Point3<Ty>& rhs) {
  return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template<typename Ty> inline
Point3<Ty> operator*(const Point3<Ty>& p, const Ty& s) {
  return { p.x * s, p.y * s, p.z * s };
}

/**
 *  \brief View an 3-element array as Point3<Ty>
 */
template <typename Ty>
struct Point3V {
  Point3V(Ty* const d) : data(d) { }
  Point3V(Ty* const d, const Point3<Ty>& p) : data(d) { *this = p; }
  Point3V(Ty* const d, const Ty x, const Ty y, const Ty z) : data(d) { 
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  Point3V<Ty>& operator=(const Point3<Ty>& rhs) {
    data[0] = Ty(rhs.x);
    data[1] = Ty(rhs.y);
    data[2] = Ty(rhs.z);
    return *this;
  }

  Ty* data{nullptr}; // pointer to 3 components
};

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
