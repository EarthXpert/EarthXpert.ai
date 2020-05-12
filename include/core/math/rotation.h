/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include "opencv2/core/mat.hpp"

namespace watxtract::math {
/**
 * 2D rotation
 */

/**
 * 3D rotation
 */
void WXCORE_EXPORTS EulerXYZToMatrix(const double *angles, cv::Mat1d &mat3x3);
} // namespace watxtract::rotation
