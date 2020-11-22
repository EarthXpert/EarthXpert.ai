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

constexpr double kFOOT2METER = 0.3048;
constexpr double kMETER2FOOT = 1.0 / kFOOT2METER;
constexpr double kUSFT2METER = 0.3048006096;
constexpr double kMETER2USFT = 1.0 / kUSFT2METER;

constexpr inline double Foot2Meter(const double foot) {
  return foot * kFOOT2METER;
}
constexpr inline double Meter2Foot(const double meter) {
  return meter * kMETER2FOOT;
}

constexpr inline double USFoot2Meter(const double foot) {
  return foot * kUSFT2METER;
}
constexpr inline double Meter2USFoot(const double meter) {
  return meter * kMETER2USFT;
}

constexpr double kWGS84_A = 6378137.0;  // equatorial radius [unit: meter]
constexpr double kWGS84_B = 6356752.3142; // polar radius
constexpr double kWGS84_ECCSQ = 6.69437999014e-3; // first eccentricity squared

constexpr int kTileDim = 256;   // tile size of raster image

constexpr double kTolDEG = 1.0e-8; // [arc degree] ~= 1mm
constexpr double kTolRAD = kTolDEG * kDEG2RAD; // [radian] ~= 1mm

constexpr double kTolCM = 1.0e-2; // [meter] = 1cm
constexpr double kTolMM = 1.0e-3; // [meter] = 1mm
constexpr double kTolUM = 1.0e-6; // [meter] = 1um

constexpr double kTolPIX = 1.0e-2; // [0.01 pixel]
} // namespace earthxpert
