/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include "core/math/constants.h"

namespace earthxpert {
// \todo units template
inline Point3d Deg2Rad(const Point3d& deg) {
  return { deg.x * kDEG2RAD, deg.y * kDEG2RAD, deg.z };
}

inline Point3d Rad2Deg(const Point3d& rad) {
  return { rad.x * kRAD2DEG, rad.y * kRAD2DEG, rad.z };
}

inline Point3d Gon2Rad(const Point3d& deg) {
  return { deg.x * kGON2RAD, deg.y * kGON2RAD, deg.z };
}

inline Point3d Rad2Gon(const Point3d& rad) {
  return { rad.x * kRAD2GON, rad.y * kRAD2GON, rad.z };
}
} // namespace earthxpert
