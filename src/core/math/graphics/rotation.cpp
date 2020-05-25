/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/math/rotation.h"

#include <cassert>
#include <cmath>

namespace earthxpert::math {
/**
 * Convert Euler angles to a rotation matrix
 *
 * \brief Convert three Euler angles to a 3x3 rotation matrix.
 *
 * \param [in] angles three Euler angles that represent a sequential 3D
 * rotation about firstly X-axis, then the rotated Y-axis, finally the
 * doubly-rotated Z axis. [radian] 
 * \param [out] mat3x3 a 9 elemants matrix representing the corresponding 3x3 rotation.
 */
void EulerXYZToMatrix(const double *angles, cv::Mat1d &mat3x3) {
  assert(CV_64FC1 == mat3x3.type());

  const auto co = cos(angles[0]); // omega about X-axis
  const auto so = sin(angles[0]);
  const auto cp = cos(angles[1]); // phi about Y-axis
  const auto sp = sin(angles[1]);
  const auto ck = cos(angles[2]); // kappa about Z-axis
  const auto sk = sin(angles[2]);

  mat3x3(0, 0) = cp * ck;
  mat3x3(0, 1) = so * sp * ck + co * sk;
  mat3x3(0, 2) = -co * sp * ck + so * sk;
  mat3x3(1, 0) = -cp * sk;
  mat3x3(1, 1) = -so * sp * sk + co * ck;
  mat3x3(1, 2) = co * sp * sk + so * ck;
  mat3x3(2, 0) = sp;
  mat3x3(2, 1) = -so * cp;
  mat3x3(2, 2) = co * cp;
}

} // namespace earthxpert::math
