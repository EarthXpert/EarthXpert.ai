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

constexpr double kFEET2METER = 0.3048;
constexpr double kMETER2FEET = 1.0 / kFEET2METER;
constexpr double kUSFT2METER = 0.3048006096;
constexpr double kMETER2USFT = 1.0 / kUSFT2METER;

constexpr inline double Feet2Meter(const double feet) {
  return feet * kFEET2METER;
}
constexpr inline double Meter2Feet(const double meter) {
  return meter * kMETER2FEET;
}

constexpr inline double USFeet2Meter(const double feet) {
  return feet * kUSFT2METER;
}
constexpr inline double Meter2USFeet(const double meter) {
  return meter * kMETER2USFT;
}

constexpr double kWGS84_A = 6378137.0;  // equatorial radius [unit: meter]
constexpr double kWGS84_B = 6356752.3142; // polar radius
constexpr double kWGS84_ECCSQ = 6.69437999014e-3; // first eccentricity squared
} // namespace earthxpert
