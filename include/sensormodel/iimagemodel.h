/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/opencv/exopencv.h"

#include <array>
#include <utility>

namespace earthxpert {
/**
 *  The class defines interface of image product vs focal plane relationship.
 *
 *  \detail
 */
class EXSENSORMODEL_EXPORTS IImageModel {
public:
  virtual ~IImageModel() {}

  /**
   *  \brief Given a 2d position in image space,
   *  output the 2d position in focal plane coordinate system.
   *
   *  \param [in] cr 2d raster position (col, row) in image space [unit: pixel]
   *  \return Return 2d metric position (x, y) in focal plane [unit: mm] if the operation succeeds
   */
  virtual Point2d CRToXY(const Point2d &cr) const = 0;

  /**
   *  \brief Given a 2d position in focal plane coordinate system,
   *  output the 2d position in image space.
   *
   *  \param [in] xy 2d metric position (x, y) in focal plane [unit: mm]
   *  \return Return 2d raster position (col, row) in image space [unit: pixel] if the operation succeeds
   */
  virtual Point2d XYToCR(const Point2d &xy) const = 0;

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  virtual std::pair<int, int> GetImageSize() const noexcept = 0;

  /**
   *  \return Return metric size of frame (width, height) [unit: mm]
   */
  virtual std::pair<double, double> GetSensorSize() const noexcept = 0;
};
} // namespace earthxpert
