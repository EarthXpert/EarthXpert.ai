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

inline int Stride(const int n, const int stride) {
  return copysign(((abs(n) - 1) / stride + 1) * stride, n);
}
}
