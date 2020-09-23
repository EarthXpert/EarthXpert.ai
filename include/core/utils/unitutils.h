/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include "core/math/constants.h"

namespace earthxpert {
// angle unit
constexpr int kAU_DEGREE = 0;
constexpr int kAU_GON = 1;
constexpr int kAU_RADIAN = 2;

// linear unit
constexpr int kLU_FEET = 0;
constexpr int kLU_METER = 1;
constexpr int kLU_USFEET = 2;

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

/**
 *  \tparam angleUnit angle unit of horizontal coordinates in ground: degree, gon, radian
 */
template<int unit> struct AngleUnit {
  static inline Point3d ToRadian(const Point3d& point) {
    if constexpr(kAU_DEGREE == unit) {
      return Deg2Rad(point);
    }
    else if constexpr(kAU_GON == unit) {
      return Gon2Rad(point);
    }
    else {
      return point;
    }
  }

  static inline Point3d FromRadian(const Point3d& point) {
    if constexpr(kAU_DEGREE == unit) {
      return Rad2Deg(point);
    }
    else if constexpr(kAU_GON == unit) {
      return Rad2Gon(point);
    }
    else {
      return point;
    }
  }
};

inline Point3d Feet2Meter(const Point3d& feet) {
  return { feet.x * kFEET2METER, feet.y * kFEET2METER, feet.z * kFEET2METER };
}
inline Point3d Meter2Feet(const Point3d& meter) {
  return { meter.x * kMETER2FEET, meter.y * kMETER2FEET, meter.z * kMETER2FEET };
}

inline Point3d USFeet2Meter(const Point3d& feet) {
  return { feet.x * kUSFT2METER, feet.y * kUSFT2METER, feet.z * kUSFT2METER };
}
inline Point3d Meter2USFeet(const Point3d& meter) {
  return { meter.x * kMETER2USFT, meter.y * kMETER2USFT, meter.z * kMETER2USFT };
}

/**
 *  \tparam linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template<int unit> struct LinearUnit {
  static inline Point3d ToMeter(const Point3d& point) {
    if constexpr(kLU_FEET == unit) {
      return Feet2Meter(point);
    }
    else if constexpr(kLU_USFEET == unit) {
      return USFeet2Meter(point);
    }
    else {
      return point;
    }
  }

  static inline Point3d FromMeter(const Point3d& point) {
    if constexpr(kLU_FEET == unit) {
      return Meter2Feet(point);
    }
    else if constexpr(kLU_USFEET == unit) {
      return Meter2USFeet(point);
    }
    else {
      return point;
    }
  }
};
} // namespace earthxpert
