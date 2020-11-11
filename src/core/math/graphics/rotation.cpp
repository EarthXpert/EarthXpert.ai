/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/rotation.h"

#include <cassert>

namespace earthxpert::math {
/**
 *  Convert Euler angles to a rotation matrix usually in object-centered system
 *
 *  \param [in] angles three Euler angles (omega, phi, kappa) that represent 
 *  a sequential 3D rotation about firstly X-axis, then the rotated Y-axis, 
 *  finally the doubly-rotated Z axis. [radian] 
 *  \param [out] mat3x3 a 9 elemants matrix representing the corresponding 3x3 rotation.
 */
void EulerXYZToMatrix(const double *angles, cv::Mat1d &mat3x3) {
  assert(angles);
  assert(CV_64FC1 == mat3x3.type());
  const auto [so, co, sp, cp, sk, ck] = sincos6(angles);
  //const auto [[so, co], [sp, cp], [sk, ck]] = sincos3(angles);
  const double so_sp = so * sp;
  const double co_sp = co * sp;

  mat3x3(0, 0) = cp * ck;
  mat3x3(0, 1) = so_sp * ck + co * sk;
  mat3x3(0, 2) = -co_sp * ck + so * sk;
  mat3x3(1, 0) = -cp * sk;
  mat3x3(1, 1) = -so_sp * sk + co * ck;
  mat3x3(1, 2) = co_sp * sk + so * ck;
  mat3x3(2, 0) = sp;
  mat3x3(2, 1) = -so * cp;
  mat3x3(2, 2) = co * cp;
}

void EulerYXZToMatrix(const double* angles, cv::Mat1d& mat3x3) {
  assert(angles);
  assert(CV_64FC1 == mat3x3.type());
  const auto [sr, cr, sp, cp, sy, cy] = sincos6(angles);
  const double cp_cy = cp * cy;
  const double sp_sy = sp * sy;
  const double sp_cy = sp * cy;
  const double cp_sy = cp * sy;

  mat3x3(0, 0) = cp_cy + sr * sp_sy;
  mat3x3(0, 1) = cr * sy;
  mat3x3(0, 2) = -sp_cy + sr * cp_sy;
  mat3x3(1, 0) = -cp_sy + sr * sp_cy;
  mat3x3(1, 1) = cr * cy;
  mat3x3(1, 2) = sp_sy + sr * cp_cy;
  mat3x3(2, 0) = cr * sp;
  mat3x3(2, 1) = -sr;
  mat3x3(2, 2) = cr * cp;
}

void EulerZXYToMatrix(const double* angles, cv::Mat1d& mat3x3)
{
  assert(angles);
  assert(CV_64FC1 == mat3x3.type());
  const auto [sr, cr, sp, cp, sy, cy] = sincos6(angles);
  const double cp_cy = cp * cy;
  const double sp_sy = sp * sy;
  const double cp_sy = cp * sy;
  const double sp_cy = sp * cy;

  mat3x3(0, 0) = cp_cy - sr * sp_sy;
  mat3x3(0, 1) = cp_sy + sr * sp_cy;
  mat3x3(0, 2) = -cr * sp;
  mat3x3(1, 0) = -cr * sy;
  mat3x3(1, 1) = cr * cy;
  mat3x3(1, 2) = sr;
  mat3x3(2, 0) = sp_cy + sr * cp_sy;
  mat3x3(2, 1) = sp_sy - sr * cp_cy;
  mat3x3(2, 2) = cr * cp;
}

/**
 *  Convert Euler angles to a rotation matrix usually in camera-centered system
 *
 *  \param [in] angles three Euler angles (roll, pitch, yaw) that represent 
 *  a sequential 3D rotation about firstly Z-axis, then the rotated Y-axis, 
 *  finally the doubly-rotated X axis. [radian]
 * \param [out] mat3x3 a 9 elemants matrix representing the corresponding 3x3 rotation.
 */
void EulerZYXToMatrix(const double* angles, cv::Mat1d& mat3x3) {
  assert(angles);
  assert(CV_64FC1 == mat3x3.type());
  const auto [sr, cr, sp, cp, sy, cy] = sincos6(angles); 
  const double sp_cy = sp * cy;
  const double sp_sy = sp * sy;

  mat3x3(0, 0) = cp * cy;
  mat3x3(0, 1) = cp * sy;
  mat3x3(0, 2) = -sp;
  mat3x3(1, 0) = -cr * sy + sr * sp_cy;
  mat3x3(1, 1) = cr * cy + sr * sp_sy;
  mat3x3(1, 2) = sr * cp;
  mat3x3(2, 0) = sr * sy + cr * sp_cy;
  mat3x3(2, 1) = -sr * cy + cr * sp_sy;
  mat3x3(2, 2) = cr * cp;
}

/**
 *  Convert a rotation matrix to Euler angles usually in object-centered system
 *
 *  \param [in] mat3x3 a 9 elemants matrix representing the 3x3 rotation.
 *  \param [out] angles three Euler angles (omega, phi, kappa) that represent
 *  a sequential 3D rotation about firstly X-axis, then the rotated Y-axis,
 *  finally the doubly-rotated Z axis. [radian]
 */
void MatrixToEulerXYZ(const cv::Mat1d& mat3x3, double* const angles) {
  assert(CV_64FC1 == mat3x3.type());
  assert(angles);

  angles[1] = asin(mat3x3(2, 0));

  if (fabs(mat3x3(0, 0)) > 0 && fabs(mat3x3(2, 2)) > 0) {
    angles[0] = atan2(-mat3x3(2, 1), mat3x3(2, 2));
    angles[2] = atan2(-mat3x3(1, 0), mat3x3(0, 0));
  }
  else {
    angles[0] = 0;
    angles[2] = atan2(-mat3x3(0, 1), mat3x3(1, 1));
  }
}
} // namespace earthxpert::math
