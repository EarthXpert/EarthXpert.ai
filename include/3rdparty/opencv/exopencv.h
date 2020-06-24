/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "opencv2/core/base.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

namespace earthxpert {      // \todo ::thirdparty
using Point2f = cv::Point2f;
using Point3f = cv::Point3f;
using Point2d = cv::Point2d;
using Point3d = cv::Point3d;

namespace thirdparty {
/**
 *  \brief calculate the squared norm of a vector
 */
template<typename PointTy> inline double normL2Sqr(const PointTy& pt) {
  // requires ddot()
  return pt.ddot(pt);
}
}
} // namespace earthxpert::thirdparty
