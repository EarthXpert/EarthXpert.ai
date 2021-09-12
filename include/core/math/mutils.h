/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/math/math.h"

#include <cmath>
#include <type_traits>

namespace earthxpert {
//
template<typename Ty, Ty val, typename = std::enable_if<std::is_integral_v<Ty>, void>::type>
struct NotVal {
  bool operator()(const Ty v) { return val != v; }
};

template<typename Ty = double>
struct NotZero {
  bool operator()(const Ty v) { return Ty(0) != v; }
};

/**
 *  \brief compare values, the first value is assgined to be the third
 *  iff the latter is smaller, and the second value is assigned to
 *  be the third iff the latter is larger.
 *
 *  \param [in/out] sval to be assgined to be the third iff the latter is smaller
 *  \param [in/out] lval to be assgined to be the third iff the latter is larger
 *  \param [in] val the value used to update sval & lval.
 */

template<> inline void Smaller<Point2d>(Point2d& sval, const Point2d& val) {
  Smaller(sval.x, val.x);
  Smaller(sval.y, val.y);
}

template<> inline void Larger<Point2d>(Point2d& lval, const Point2d& val) {
  Larger(lval.x, val.x);
  Larger(lval.y, val.y);
}

template<> inline void MinMax<Point2d>(Point2d& sval, Point2d& lval, const Point2d& val) {
  MinMax(sval.x, lval.x, val.x);
  MinMax(sval.y, lval.y, val.y);
}

template<> inline void Smaller<Point3d>(Point3d& sval, const Point3d& val) {
  Smaller(sval.x, val.x);
  Smaller(sval.y, val.y);
  Smaller(sval.z, val.z);
}

template<> inline void Larger<Point3d>(Point3d& lval, const Point3d& val) {
  Larger(lval.x, val.x);
  Larger(lval.y, val.y);
  Larger(lval.z, val.z);
}

template<> inline void MinMax<Point3d>(Point3d& sval, Point3d& lval, const Point3d& val) {
  MinMax(sval.x, lval.x, val.x);
  MinMax(sval.y, lval.y, val.y);
  MinMax(sval.z, lval.z, val.z);
}

double EXCORE_EXPORTS TruncateOffsetDigits(double off, int nunDigits);
double EXCORE_EXPORTS TruncateScaleDigits(double off, int nunDigits);

std::pair<double, double> EXCORE_EXPORTS 
CalcOffsetScale(const std::vector<double>& arr, int offDigits, int sclDigits);

void EXCORE_EXPORTS CalcStatistics(const std::vector<double>& arr, std::array<double, 4>& stats);
}
