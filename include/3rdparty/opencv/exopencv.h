/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <cmath>
#include <type_traits>

namespace x2d = cv::xfeatures2d;

namespace earthxpert {      // \todo ::thirdparty
// CV_8U CV_8S CV_16U CV_16S CV_32S CV_32F CV_64F CV_16F
constexpr size_t kNumDepth = 8;

template<typename Ty> inline
cv::Point_<Ty> operator+(const cv::Point_<Ty>& p, const Ty v) {
  return {p.x + v, p.y + v};
}

template<typename Ty> inline
cv::Point_<Ty> operator-(const cv::Point_<Ty>& p, const Ty v) {
  return { p.x - v, p.y - v };
}

using Point2f = cv::Point2f;
using Point3f = cv::Point3f;
using Point2d = cv::Point2d;
using Point3d = cv::Point3d;
using Point2i = cv::Point2i;
using Point3i = cv::Point3i;
using Rect2i = cv::Rect2i;

inline Point2d xabs(const Point2d& p) {
  return Point2d(fabs(p.x), fabs(p.y));
}

inline Point3d xabs(const Point3d& p) {
  return Point3d(fabs(p.x), fabs(p.y), fabs(p.z));
}

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
