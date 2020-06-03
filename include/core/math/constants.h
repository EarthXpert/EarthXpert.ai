/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include <cmath>

namespace earthxpert {
// \todo units
constexpr double kDEG2RAD = M_PI / 180.0;
constexpr double kRAD2DEG = 180.0 / M_PI;
constexpr double kGON2RAD = M_PI / 200.0;
constexpr double kRAD2GON = 200.0 / M_PI;

constexpr inline double Deg2Rad(const double deg) {
  return deg * kDEG2RAD;
}
constexpr inline double Rad2Deg(const double rad) {
  return rad * kRAD2DEG;
}

constexpr inline double Gon2Rad(const double gon) {
  return gon * kGON2RAD;
}
constexpr inline double Rad2Gon(const double rad) {
  return rad * kRAD2GON;
}

constexpr double kWGS84_A = 6378137.0;  // equatorial radius [unit: meter]
constexpr double kWGS84_B = 6356752.3142; // polar radius
constexpr double kWGS84_ECCSQ = 6.69437999014e-3; // first eccentricity squared
} // namespace earthxpert
