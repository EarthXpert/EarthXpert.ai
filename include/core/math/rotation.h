/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "opencv2/core/mat.hpp"

namespace earthxpert::math {
/**
 * 2D rotation
 */

/**
 * 3D rotation
 */
void EXCORE_EXPORTS EulerXYZToMatrix(const double *angles, cv::Mat1d &mat3x3);
} // namespace earthxpert::rotation
