/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <cmath>

namespace earthxpert {
// \return Return exponent of 2
constexpr inline size_t exp2(const size_t n) {
  return size_t(1) << n;
}

/**
 *  \brief compare two values, the first value is assgined to be the second
 *  iff the latter is smaller.
 */
template <typename Ty> inline void Smaller(Ty& sval, const Ty& val) {
  if (sval > val) sval = val;
}

/**
 *  \brief compare two values, the first value is assgined to be the second
 *  iff the latter is larger.
 */
template <typename Ty> inline void Larger(Ty& lval, const Ty& val) {
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
template <typename Ty> inline void MinMax(Ty& sval, Ty& lval, const Ty& val) {
  Smaller(sval, val);
  Larger(lval, val);
}

inline double LinearInterp(const double v1, const double v2, const double v3,
  const double v4, const double xf, const double yf) {
    return (1.0 - yf) * ((1.0 - xf) * v1 + xf * v2) +
      yf * ((1.0 - xf) * v3 + xf * v4);
};

/**
 *  \brief  make a number being a multiple of a stride, and extend away from 0
 */
inline int Stride(const int n, const int stride) {
  return copysign(((abs(n) - 1) / stride + 1) * stride, n);
}
}
