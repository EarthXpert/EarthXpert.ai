/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/nsimd/exnsimd.h"
#include "3rdparty/simd/exsimd.h"
#include "3rdparty/xsimd/exxsimd.h"

#include <mmintrin.h>  // __m64
#include <xmmintrin.h>  // __m128
#include <emmintrin.h>  // SSE 128: __m128i, __m128d
#include <pmmintrin.h>  // SSE3
#include <smmintrin.h>  // SSE4.1
#include <nmmintrin.h>  // SSE4.2
#include <immintrin.h>  // AVX 256: __m256, __m256d, __m256i
#include <tmmintrin.h>  // 
#include <wmmintrin.h>  // AES and PCLMULQDQ
#include <zmmintrin.h>  // AVX 512: __m512, __m512d, __m512i
#include <ammintrin.h>  // AMD

namespace earthxpert {
inline double LinearInterp_Simd(const double v1, const double v2,
  const double v3, const double v4, const double xf, const double yf) {
  return (1.0 - yf) * ((1.0 - xf) * v1 + xf * v2) +
    yf * ((1.0 - xf) * v3 + xf * v4);
}
}
