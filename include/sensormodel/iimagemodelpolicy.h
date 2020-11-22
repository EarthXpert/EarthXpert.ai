/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert {
/**
 *  The interface IImageModelPolicy defines the transformation
 *  between the focal plane and the raster imagery.
 */
class EXSENSORMODEL_EXPORTS IImageModelPolicy {
public:
  enum ImageModelPolicyType {
    eIM_DIGITAL, //
    eIM_FILM
  };
  using impt = ImageModelPolicyType;

  virtual ~IImageModelPolicy() {}

  /**
   *  \brief Given a 2d position in image coordinate system,
   *  output the 2d position in sensor focal plane coordinate system.
   *
   *  \param [in] image 2d position (col, row) in imagery coordinate system
   *  \return Return 2d position (x, y) in sensor focal plane coordinate
   * system if the operation succeeds
   */
  virtual Point2d ImageToSensor(const Point2d &image) const = 0;

  /**
   *  \brief Given a 2d position in sensor focal plane coordinate system,
   *  output the 2d position in image coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
   * system
   *  \return Return 2d position (col, row) in imagery coordinate system if the operation succeeds
   */
  virtual Point2d SensorToImage(const Point2d &sensor) const = 0;

  /**
   * \return Return type of image model policy
   */
  virtual ImageModelPolicyType GetImageModelPolicyType() const noexcept = 0;

  virtual std::pair<int, int> GetImageSize() const noexcept = 0;
};
}