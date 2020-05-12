/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/epipolarrectmarshaller.h"
#include "epipolarmodel/epipolarrect.h"
#include <cereal/types/array.hpp>
#include <iostream>

using watxtract::EpipolarRect;
using watxtract::EpipolarRectMarshaller;

/**
 *  brief constructor construct marshaller for epirect
 *
 *  \param [in] the pointer to the epipolar rect
 *
 */
EpipolarRectMarshaller::EpipolarRectMarshaller(const EpipolarRect *epiRect)
    : mXOffset(epiRect->GetXOffset()), mYOffset(epiRect->GetYOffset()),
      mScale(epiRect->GetScale()), mRotation(epiRect->GetRotation()),
      mHeight(epiRect->GetHeight()), mNumRows(epiRect->GetNumRows()) {
  auto origin3d = epiRect->GetOrigin();
  mOrigin = {origin3d.x, origin3d.y, origin3d.z};
}

/**
 *  brief get the pointer of the epipolar rect
 *
 *  \return the pointer to the epipolar rect
 *
 */
std::unique_ptr<EpipolarRect> EpipolarRectMarshaller::GetRect() {
  Point3d origin = Point3d(mOrigin[0], mOrigin[1], mOrigin[2]);
  // return std::make_unique<EpipolarRect>(mXOffset, mYOffset, mScale,
  //                             mRotation, mHeight, mNumRows, origin);
  return std::unique_ptr<EpipolarRect>();
}
