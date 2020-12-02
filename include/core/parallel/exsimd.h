/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/nsimd/exnsimd.h"
#include "3rdparty/simd/exsimd.h"
#include "3rdparty/xsimd/exxsimd.h"

#include <intrin.h>  //
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
// a[0] + a[1] + a[2] + a[3] 
inline __m128 _Sum_Simd(__m128 a) {
  __m128 r = _mm_add_ps(a, _mm_movehl_ps(a, a));
  return _mm_add_ss(r, _mm_shuffle_ps(r, r, 1));
}

// c = a . b    
// a[0] * b[1] + a[0] * b[1] + a[0] * b[1] 
inline __m128 Dot_Simd(__m128 a, __m128 b) {
  __m128 s = _mm_mul_ps(a, b);
  __m128 r = _mm_add_ss(s, _mm_movehl_ps(s, s));
  return _mm_add_ss(r, _mm_shuffle_ps(r, r, 1));
}

// c = a x b
// (a[1] * b[2] - a[2] * b[1]), (a[2] * b[0] - a[0] * b[2]), (a[0] * b[1] - a[1] * b[0])
inline __m128 Cross_Simd(__m128 a, __m128 b) {
  __m128 ea = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 eb = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
  __m128 xa = _mm_mul_ps(ea, eb);

  a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
  b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 xb = _mm_mul_ps(a, b);

  return _mm_sub_ps(xa, xb);
}

inline double LinearInterp_Simd(const double v1, const double v2,
  const double v3, const double v4, const double xf, const double yf) {
  return (1.0 - yf) * ((1.0 - xf) * v1 + xf * v2) +
    yf * ((1.0 - xf) * v3 + xf * v4);
}
}
