/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/utils/typehelper.h"
#include "sensormodel/rationalmodel.h"

#include <bitset>

namespace earthxpert {
void EXSENSORMODEL_EXPORTS CalcTerms(double u, double v, double w, double* const uvw);

inline void CalcTerms(const Point3d& uvw, double* const LPH) {
  CalcTerms(uvw.x, uvw.y, uvw.z, LPH);
}

void EXSENSORMODEL_EXPORTS CalcTerms(const Point3d& uvw, 
  double* const LPH, const std::bitset<rpc::NumCoef>& coefFlags);

// Partial derivatives of the polynomial terms w.r.t u, v, w.
// \todo
inline void Duvw_du(const double u, const double v, const double w, double* duvw_du) {
  // 0.0,         1.0, 0.0, 0.0,        v,           w,     0.0,
  // 2.0 * u,     0.0, 0.0, v * w,      3.0 * u * u, v * v, w * w,
  // 2.0 * u * v, 0.0, 0.0, 2.0 * u * w, 0.0,        0.0 
  duvw_du[0] = 0.0; duvw_du[1] = 1.0; duvw_du[2] = 0.0; duvw_du[3] = 0.0;
  duvw_du[4] = v; duvw_du[5] = w; duvw_du[6] = 0.0; duvw_du[7] = 2.0 * u;
  duvw_du[8] = 0.0; duvw_du[9] = 0.0; duvw_du[10] = v * w; duvw_du[11] = 3.0 * u * u;
  duvw_du[12] = v * v; duvw_du[13] = w * w; duvw_du[14] = 2.0 * u * v; duvw_du[15] = 0.0;
  duvw_du[16] = 0.0; duvw_du[17] = 2.0 * u * w; duvw_du[18] = 0.0; duvw_du[19] = 0.0;
}

inline void Duvw_dv(const double u, const double v, const double w, double* duvw_dv) {
  // 0.0,   0.0,         1.0,   0.0,   u,           0.0,         w,
  // 0.0,   2.0 * v,     0.0,   u * w, 0.0,         2.0 * u * v, 0.0,
  // u * u, 3.0 * v * v, w * w, 0.0,   2.0 * v * w, 0.0
  duvw_dv[0] = 0.0; duvw_dv[1] = 0.0; duvw_dv[2] = 1.0; duvw_dv[3] = 0.0;
  duvw_dv[4] = u; duvw_dv[5] = 0.0; duvw_dv[6] = w; duvw_dv[7] = 0.0;
  duvw_dv[8] = 2.0 * v; duvw_dv[9] = 0.0; duvw_dv[10] = u * w; duvw_dv[11] = 0.0;
  duvw_dv[12] = 2.0 * u * v; duvw_dv[13] = 0.0; duvw_dv[14] = u * u; duvw_dv[15] = 3.0 * v * v;
  duvw_dv[16] = w * w; duvw_dv[17] = 0.0; duvw_dv[18] = 2.0 * v * w; duvw_dv[19] = 0.0;
}

inline void Duvw_dw(const double u, const double v, const double w, double* duvw_dw) {
  // 0.0, 0.0, 0.0,         1.0,   0.0,   u,          v,
  // 0.0, 0.0, 2.0 * w,     u * v, 0.0,   0.0,        2.0 * u * w,
  // 0.0, 0.0, 2.0 * v * w, u * u, v * v, 3.0 * w * w
  duvw_dw[0] = 0.0; duvw_dw[1] = 0.0; duvw_dw[2] = 0.0; duvw_dw[3] = 1.0;
  duvw_dw[4] = 0.0; duvw_dw[5] = u; duvw_dw[6] = v; duvw_dw[7] = 0.0;
  duvw_dw[8] = 0.0; duvw_dw[9] = 2.0 * w; duvw_dw[10] = u * v; duvw_dw[11] = 0.0;
  duvw_dw[12] = 0.0; duvw_dw[13] = 2.0 * u * w; duvw_dw[14] = 0.0; duvw_dw[15] = 0.0;
  duvw_dw[16] = 2.0 * v * w; duvw_dw[17] = u * u; duvw_dw[18] = v * v; duvw_dw[19] = 3.0 * w * w;
}

inline Tuple6d UVW(const double u, const double v, const double w) noexcept {
  return {u*u, v*v, w*w, u*v, u*w, v*w};
}

inline void Duvw_du(const double u, const double v, const double w,
  const double u2, const double v2, const double w2, 
  const double uv, const double uw, const double vw, double* duvw_du) {
  duvw_du[0] = 0.0; duvw_du[1] = 1.0; duvw_du[2] = 0.0; duvw_du[3] = 0.0;
  duvw_du[4] = v; duvw_du[5] = w; duvw_du[6] = 0.0; duvw_du[7] = 2.0 * u;
  duvw_du[8] = 0.0; duvw_du[9] = 0.0; duvw_du[10] = vw; duvw_du[11] = 3.0 * u2;
  duvw_du[12] = v2; duvw_du[13] = w2; duvw_du[14] = 2.0 * uv; duvw_du[15] = 0.0;
  duvw_du[16] = 0.0; duvw_du[17] = 2.0 * uw; duvw_du[18] = 0.0; duvw_du[19] = 0.0;
}

inline void Duvw_dv(const double u, const double v, const double w,
  const double u2, const double v2, const double w2,
  const double uv, const double uw, const double vw, double* duvw_dv) {
  duvw_dv[0] = 0.0; duvw_dv[1] = 0.0; duvw_dv[2] = 1.0; duvw_dv[3] = 0.0;
  duvw_dv[4] = u; duvw_dv[5] = 0.0; duvw_dv[6] = w; duvw_dv[7] = 0.0;
  duvw_dv[8] = 2.0 * v; duvw_dv[9] = 0.0; duvw_dv[10] = uw; duvw_dv[11] = 0.0;
  duvw_dv[12] = 2.0 * uv; duvw_dv[13] = 0.0; duvw_dv[14] = u2; duvw_dv[15] = 3.0 * v2;
  duvw_dv[16] = w2; duvw_dv[17] = 0.0; duvw_dv[18] = 2.0 * vw; duvw_dv[19] = 0.0;
}

inline void Duvw_dw(const double u, const double v, const double w,
  const double u2, const double v2, const double w2,
  const double uv, const double uw, const double vw, double* duvw_dw) {
  duvw_dw[0] = 0.0; duvw_dw[1] = 0.0; duvw_dw[2] = 0.0; duvw_dw[3] = 1.0;
  duvw_dw[4] = 0.0; duvw_dw[5] = u; duvw_dw[6] = v; duvw_dw[7] = 0.0;
  duvw_dw[8] = 0.0; duvw_dw[9] = 2.0 * w; duvw_dw[10] = uv; duvw_dw[11] = 0.0;
  duvw_dw[12] = 0.0; duvw_dw[13] = 2.0 * uw; duvw_dw[14] = 0.0; duvw_dw[15] = 0.0;
  duvw_dw[16] = 2.0 * vw; duvw_dw[17] = u2; duvw_dw[18] = v2; duvw_dw[19] = 3.0 * w2;
}

/**
 *  \code
    Duvw_du(u, v, w, duvw_du);
    Duvw_dv(u, v, w, duvw_dv);
 */
inline void Duvw_duv(const double u, const double v, const double w,
  double* duvw_du, double* duvw_dv) {
  const auto [u2, v2, w2, uv, uw, vw] = UVW(u, v, w);

  Duvw_du(u, v, w, u2, v2, w2, uv, uw, vw, duvw_du);
  Duvw_dv(u, v, w, u2, v2, w2, uv, uw, vw, duvw_dv);
}

/**
 *  \code
    Duvw_du(u, v, w, duvw_du);
    Duvw_dv(u, v, w, duvw_dv);
    Duvw_dw(u, v, w, duvw_dw);
 */
inline void Duvw_duvw(const double u, const double v, const double w,
  double* duvw_du, double* duvw_dv, double* duvw_dw) {
  const auto [u2, v2, w2, uv, uw, vw] = UVW(u, v, w);

  Duvw_du(u, v, w, u2, v2, w2, uv, uw, vw, duvw_du);
  Duvw_dv(u, v, w, u2, v2, w2, uv, uw, vw, duvw_dv);
  Duvw_dw(u, v, w, u2, v2, w2, uv, uw, vw, duvw_dw);
}

// Partial derivatives of the numerator and denominator polynomials w.r.t u, v, w.
// unrolling the loop for performance with 10 out of the 20 terms
inline double D_du(const double* c, const double* duvw_du) {
  return c[1] + c[4] * duvw_du[4] + c[5] * duvw_du[5] + c[7] * duvw_du[7] +
    c[10] * duvw_du[10] + c[11] * duvw_du[11] + c[12] * duvw_du[12] +
    c[13] * duvw_du[13] + c[14] * duvw_du[14] + c[17] * duvw_du[17];
}

inline double D_dv(const double* c, const double* duvw_dv) {
  return c[2] + c[4] * duvw_dv[4] + c[6] * duvw_dv[6] + c[8] * duvw_dv[8] +
    c[10] * duvw_dv[10] + c[12] * duvw_dv[12] + c[14] * duvw_dv[14] +
    c[15] * duvw_dv[15] + c[16] * duvw_dv[16] + c[18] * duvw_dv[18];
}

inline double D_dw(const double* c, const double* duvw_dw) {
  return c[3] + c[5] * duvw_dw[5] + c[6] * duvw_dw[6] + c[9] * duvw_dw[9] +
    c[10] * duvw_dw[10] + c[13] * duvw_dw[13] + c[16] * duvw_dw[16] +
    c[17] * duvw_dw[17] + c[18] * duvw_dw[18] + c[19] * duvw_dw[19];
}

inline std::pair<double, double> D_duv(const double* c, 
  const double* duvw_du, const double* duvw_dv) {
  return { D_du(c, duvw_du), D_dv(c, duvw_dv) };
}

inline Tuple3d D_duvw(const double* c, const double* duvw_du,
  const double* duvw_dv, const double* duvw_dw) {
  return { D_du(c, duvw_du), D_dv(c, duvw_dv), D_dw(c, duvw_dw) };
}
}
