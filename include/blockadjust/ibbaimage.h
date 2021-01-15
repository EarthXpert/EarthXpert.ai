/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/exbba.h"
#include "3rdparty/opencv/exopencv.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IBbaImage defines the behaviours of image bba info.
 */
class EXBBA_EXPORTS IBbaImage {
public:
  virtual ~IBbaImage() = default;

  bool operator==(const IBbaImage& rhs) const = delete;
  bool operator!=(const IBbaImage& rhs) const = delete;

  virtual bool ImageToGround(const Point2d& image, Point3d& ground) const = 0;
  virtual bool GroundToImage(const Point3d& ground, Point2d& image) const = 0;

  virtual void UpdateModel() = 0;
};

using UPIBbaImage = std::unique_ptr<IBbaImage>;
using SPIBbaImage = std::shared_ptr<IBbaImage>;
} // namespace earthxpert
