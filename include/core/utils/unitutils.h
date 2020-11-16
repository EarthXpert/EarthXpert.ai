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
constexpr int kLU_FOOT = 0;   // International Foot
constexpr int kLU_METER = 1;
constexpr int kLU_USFOOT = 2; // US Survey Foot

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

inline Point3d Foot2Meter(const Point3d& foot) {
  return { foot.x * kFOOT2METER, foot.y * kFOOT2METER, foot.z * kFOOT2METER };
}
inline Point3d Meter2Foot(const Point3d& meter) {
  return { meter.x * kMETER2FOOT, meter.y * kMETER2FOOT, meter.z * kMETER2FOOT };
}

inline Point3d USFoot2Meter(const Point3d& foot) {
  return { foot.x * kUSFT2METER, foot.y * kUSFT2METER, foot.z * kUSFT2METER };
}
inline Point3d Meter2USFoot(const Point3d& meter) {
  return { meter.x * kMETER2USFT, meter.y * kMETER2USFT, meter.z * kMETER2USFT };
}

/**
 *  \tparam linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template<int unit> struct LinearUnit {
  static inline Point3d ToMeter(const Point3d& point) {
    if constexpr(kLU_FOOT == unit) {
      return Foot2Meter(point);
    }
    else if constexpr(kLU_USFOOT == unit) {
      return USFoot2Meter(point);
    }
    else {
      return point;
    }
  }

  static inline Point3d FromMeter(const Point3d& point) {
    if constexpr(kLU_FOOT == unit) {
      return Meter2Foot(point);
    }
    else if constexpr(kLU_USFOOT == unit) {
      return Meter2USFoot(point);
    }
    else {
      return point;
    }
  }
};
} // namespace earthxpert
