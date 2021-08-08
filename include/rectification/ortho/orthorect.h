/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/ortho/exortho.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert {
/**
 *  The class OrthoRect defines similarity transform between map projection 
 *  and raster coordinate system of ortho image.
 * 
 *  \ref Geocoding
 */
class EXORTHO_EXPORTS OrthoRect {
public:
  ~OrthoRect() = default;

private:
  Point2d mOrigin{};    // coordinate at upper-left corner of the UL pixel
  Point2d mPixSize{};   // pixel ground resolution
  double mRotation{ 0.0 }; // rotation [rad]

  // cached data for better perforamnce
  double mCosRot{};
  double mSinRot{};
};
}
