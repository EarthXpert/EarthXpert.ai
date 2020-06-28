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

#include <type_traits>

namespace earthxpert {      // \todo ::thirdparty
using Point2f = cv::Point2f;
using Point3f = cv::Point3f;
using Point2d = cv::Point2d;
using Point3d = cv::Point3d;

template<typename Ty, typename = std::enable_if_t<std::is_arithmetic_v<Ty>>>
cv::Point3_<Ty> Point2To3(const cv::Point_<Ty>& point) {
  return { point.x, point.y, 0 };
};

template<typename Ty, typename = std::enable_if_t<std::is_arithmetic_v<Ty>>>
cv::Point_<Ty> Point3To2(const cv::Point3_<Ty>& point) {
  return { point.x, point.y };
};

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
