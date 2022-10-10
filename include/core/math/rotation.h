/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/eigen/exeigen.h"

#include "core/math/math.h"
#include "core/utils/typehelper.h"
#include "opencv2/core/mat.hpp"

#include <algorithm>
#include <tuple>
#include <utility>


namespace earthxpert::math {
/**
 *  \param [in] angles three rotation angles [radian]
 */
inline Tuple6d sincos6(const double* angles) {
  return { sin(angles[0]), cos(angles[0]), 
    sin(angles[1]), cos(angles[1]), sin(angles[2]), cos(angles[2]) };
}

inline Tuple3<std::pair<double, double>> sincos3(const double* angles) {
  return { sincos(angles[0]), sincos(angles[1]), sincos(angles[2]) };
}

/**
 * 2D rotation
 */

/**
 * 3D rotation
 */
enum RotateSeq {
  eXYZ,
  eXZY,
  eYXZ,
  eYZX,
  eZXY,
  eZYX
};

void EXCORE_EXPORTS EulerXYZToMatrix(const double* angles, cv::Mat1d& mat3x3);
void EXCORE_EXPORTS EulerYXZToMatrix(const double* angles, cv::Mat1d& mat3x3);
void EXCORE_EXPORTS EulerZXYToMatrix(const double* angles, cv::Mat1d& mat3x3);
void EXCORE_EXPORTS EulerZYXToMatrix(const double* angles, cv::Mat1d& mat3x3);

void EXCORE_EXPORTS EulerXYZToMatrix(const double* angles, Eigen::Matrix3d& mat3x3);

template <RotateSeq rotSeq = eXYZ>
inline void EulerToMatrix(const double* angles, cv::Mat1d& mat3x3) {
  if constexpr (eXYZ == rotSeq) {
    EulerXYZToMatrix(angles, mat3x3);
  }
  else if constexpr (eYXZ == rotSeq) {
    EulerYXZToMatrix(angles, mat3x3);
  }
  else if constexpr (eZXY == rotSeq) {
    EulerZXYToMatrix(angles, mat3x3);
  }
  else if constexpr (eZYX == rotSeq) {
    EulerZYXToMatrix(angles, mat3x3);
  }
  else {
    throw std::invalid_argument("rotSeq");
  }
}

void EXCORE_EXPORTS MatrixToEulerXYZ(const cv::Mat1d& mat3x3, double* const angles);

template <RotateSeq rotSeq = eXYZ>
inline void MatrixToEuler(const cv::Mat1d& mat3x3, double* const angles) {
  if constexpr (eXYZ == rotSeq) {
    MatrixToEulerXYZ(mat3x3, angles);
  }
  else {
    throw std::invalid_argument("rotSeq");
  }
}
} // namespace earthxpert::rotation
