/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/geomutils.h"
#include "core/math/mutils.h"

#include <vector>

namespace earthxpert::utils {
/**
 *  \return Returns the lower-left and upper-right bounding box [pixel].
 */
std::pair<Point2d, Point2d> CalcBound(const Polygon2d& poly, const int posTfm) {
  const std::vector<PointXYd>& pts = poly.outer();

  Point2d pixMin{ DBL_MAX, DBL_MAX };
  Point2d pixMax{ -DBL_MAX, -DBL_MAX };
  for (const auto& p : pts) {
    MinMax(pixMin.x, pixMax.x, p.x());
    MinMax(pixMin.y, pixMax.y, p.y());
  }

  Point2i pMin(floor(pixMin.x), floor(pixMin.y));
  Point2i pMax(ceil(pixMax.x), ceil(pixMax.y));

  return { cv::Point2d(Stride(pMin.x, posTfm), Stride(pMin.y, posTfm)),
           cv::Point2d(Stride(pMax.x, posTfm), Stride(pMax.y, posTfm)) };
}

std::vector<Point2i> ConvertPolygon(const Polygon2d& poly) {
  std::vector<cv::Point> cvPoly;

  const std::vector<PointXYd>& pts = poly.outer();
  for (const auto& p : pts) {
    cvPoly.emplace_back(std::lround(p.x()), std::lround(p.y()));
  }

  return cvPoly;
}

Polygon2d EXCORE_EXPORTS ConvertPolygon(const RasterWindow& win) {
  Polygon2d poly;
  return poly;
}
}
