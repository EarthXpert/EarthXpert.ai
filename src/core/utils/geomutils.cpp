/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/geomutils.h"
#include "core/math/utils.h"

#include <vector>

namespace earthxpert::utils {
/**
 *  \return Returns the lower-left and upper-right bounding box [pixel].
 */
std::pair<Point2d, Point2d> CalcBound(const Polygon2d& fp, const int posTfm) {
  const std::vector<PointXYd>& fpPts = fp.outer();

  Point2d pixMin{ DBL_MAX, DBL_MAX };
  Point2d pixMax{ -DBL_MAX, -DBL_MAX };
  for (const auto& p : fpPts) {
    MinMax(pixMin.x, pixMax.x, p.x());
    MinMax(pixMin.y, pixMax.y, p.y());
  }

  Point2i pMin(floor(pixMin.x), floor(pixMin.y));
  Point2i pMax(ceil(pixMax.x), ceil(pixMax.y));

  return { cv::Point2d(Stride(pMin.x, posTfm), Stride(pMin.y, posTfm)),
           cv::Point2d(Stride(pMax.x, posTfm), Stride(pMax.y, posTfm)) };
}
}
