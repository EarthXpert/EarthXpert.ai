/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/utils/typehelper.h"
#include "3rdparty/opencv/exopencv.h"

#include <cmath>

namespace earthxpert {
/**
 *  \brief storage order of matrix elements
 */
enum MatOrder {
  eCOLMAHOR,
  eROWMAJOR
};

constexpr int DivideUp(const int num, const int den) {
  return (num - 1) / den + 1;
}

// \return Return exponent of 2
constexpr inline size_t exp2(const size_t n) {
  return size_t(1) << n;
}

/**
 *  \param [in] order the 0-based degree of polynomial
 *  \return number of coefficents of 2d polynomial
 */
constexpr int NumCoeffPoly2d(const int order) {
  return (order + 1) * (order + 2) / 2;
}

/**
 *  \param [in] angle the rotation angle [radian]
 */
inline std::pair<double, double> sincos(const double angle) {
  return { sin(angle), cos(angle) };
}

/**
 *  \brief compare two values, the first value is assgined to be the second
 *  iff the latter is smaller.
 */
template <typename Ty> inline void 
Smaller(Ty& sval, const Ty& val) {
  if (sval > val) sval = val;
}

/**
 *  \brief compare two values, the first value is assgined to be the second
 *  iff the latter is larger.
 */
template <typename Ty> inline void 
Larger(Ty& lval, const Ty& val) {
  if (lval < val) lval = val;
}

/**
 *  \brief compare values, the first value is assgined to be the third
 *  iff the latter is smaller, and the second value is assigned to
 *  be the third iff the latter is larger.
 *
 *  \param [in/out] sval to be assgined to be the third iff the latter is smaller
 *  \param [in/out] lval to be assgined to be the third iff the latter is larger
 *  \param [in] val the value used to update sval & lval.
 */
template <typename Ty> inline void 
MinMax(Ty& sval, Ty& lval, const Ty& val) {
  Smaller(sval, val);
  Larger(lval, val);
}

/**
 *  \brief weighting coefficients of linear interpolation
 */
template<int order, typename Ty>
inline Tuple4<Ty> LinearWeights(const Ty xf, const Ty yf) {
  const Ty w4 = xf * yf;
  const Ty w3 = yf - w4;  // (1 - xf) * yf
  const Ty w2 = xf - w4;  // xf * (1 - yf)
  const Ty w1 = Ty(1) - xf - yf + w4;  // (1 - xf) * (1 - yf)
  if constexpr(eCOLMAHOR == order)
    return { w1, w3, w2, w4 };
  else
    return { w1, w2, w3, w4 };
}

/**
 *  \brief bi-linear interpolation
 */
template<typename Ty=double>
inline Ty LinearInterp(const Ty v1, const Ty v2,
  const Ty v3, const Ty v4, const Ty xf, const Ty yf) {
  return Ty((1.0 - yf) * ((1.0 - xf) * v1 + xf * v2) +
    yf * ((1.0 - xf) * v3 + xf * v4));
}

inline cv::Vec2f LinearInterp(const cv::Vec2f v1, const cv::Vec2f v2,
  const cv::Vec2f v3, const cv::Vec2f v4, const double xf, const double yf) {
  return cv::Vec2f(float(LinearInterp<double>(v1[0], v2[0], v3[0], v4[0], xf, yf)),
                   float(LinearInterp<double>(v1[1], v2[1], v3[1], v4[1], xf, yf)));
}

/**
 *  \brief  make a number being a multiple of a stride, and extend away from 0
 */
inline int Stride(const int n, const int stride) {
  return ((n - 1) / stride + 1) * stride;
  //return std::copysign(((abs(n) - 1) / stride + 1) * stride, n);
}
}
