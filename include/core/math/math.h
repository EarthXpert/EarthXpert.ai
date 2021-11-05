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
 * 
 *  \param Ty scalar type
 *
 *  \param [in] xf x fraction
 *  \param [in] yf y fraction
 *  \return Returns the interpolated value
 */
template<typename Ty=double>
inline Ty LinearInterp(const Ty ul, const Ty ur,
  const Ty ll, const Ty lr, const Ty xf, const Ty yf) {
  return Ty((1 - yf) * ((1 - xf) * ul + xf * ur) +
    yf * ((1 - xf) * ll + xf * lr));
}

/**
 *  \param Ty scalar type
 *  \param dim dimension of vector elements
 *  \param Ty2 scalar type of x & y fractions
 *
 *  \param [in] xf x fraction
 *  \param [in] yf y fraction
 *  \return Returns the interpolated point
 *
 *  \ref InterpGridVec
 */
template<typename Ty, int dim = 1, typename Ty2=double>
inline cv::Vec<Ty, dim> LinearInterp(const cv::Vec<Ty, dim>& ul, 
  const cv::Vec<Ty, dim>& ur, const cv::Vec<Ty, dim>& ll, 
  const cv::Vec<Ty, dim>& lr, const Ty2 xf, const Ty2 yf) {
  return (1 - yf) * ((1 - xf) * ul + xf * ur) +
    yf * ((1 - xf) * ll + xf * lr);
}

/**
 *  \brief  make a number being a multiple of a stride, and extend away from 0
 */
inline int Stride(const int n, const int stride) {
  return ((n - 1) / stride + 1) * stride;
  //return std::copysign(((abs(n) - 1) / stride + 1) * stride, n);
}
}
