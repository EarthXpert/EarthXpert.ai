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

inline double LinearInterp(const double v1, const double v2, const double v3,
  const double v4, const double xf, const double yf) {
    return (1.0 - yf) * ((1.0 - xf) * v1 + xf * v2) +
      yf * ((1.0 - xf) * v3 + xf * v4);
};

inline int Stride(const int n, const int stride) {
  return copysign(((abs(n) - 1) / stride + 1) * stride, n);
}
}
