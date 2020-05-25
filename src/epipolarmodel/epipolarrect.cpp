/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarrect.h"
#include "core/utils/typehelper.h"

#include <cmath>
#include <limits>

using earthxpert::EpipolarRect;
using earthxpert::Point2d;
using earthxpert::Point3d;

/**
 *  Constructor.
 *
 *  \param [in] origin Origin of epi coordinate system
 *  \param [in] height Rectification height
 *  \param [in] xOffset Rectification X-offset
 *  \param [in] yOffset Rectification Y-offset
 *  \param [in] scale Rectification scale
 *  \param [in] rotation Rectification rotation (unit: rad)
 *  \param [in] numCols Number of cols of epipolar image
 *  \param [in] numRows Number of rows of epipolar image
 */
EpipolarRect::EpipolarRect(const Point3d origin, const double height, 
  const double xOffset, const double yOffset, const double scale, 
  const double rotation, const int numCols, const int numRows)
    : mOrigin(origin), mHeight(height), mXOffset(xOffset), mYOffset(yOffset), 
  mScale(scale), mRotation(rotation), mNumCols(numCols), mNumRows(numRows) {
  mInvScale = 1.0 / mScale;
  mCosRot = cos(mRotation);
  mSinRot = sin(mRotation);
}

/**
 * Test equality.
 *
 * \param [in] rhs Object to compare with
 * \return true if same or equal.
 */
bool EpipolarRect::operator==(const EpipolarRect &rhs) const {
  if (this != &rhs) {
    const Equal<double> Eq;

    if (!Eq(mXOffset, rhs.mXOffset) || !Eq(mYOffset, rhs.mYOffset))
      return false;

    if (!Eq(mHeight, rhs.mHeight) || !Eq(mScale, rhs.mScale) ||
      !Eq(mRotation, rhs.mRotation)) {
      return false;
    }

    if (mNumCols != rhs.mNumCols || mNumRows != rhs.mNumRows)
      return false;
  }
  return true;
}

/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
bool EpipolarRect::operator!=(const EpipolarRect &rhs) const {
  return !(operator==(rhs));
}

/**
 *  Epipolar to rectification projection
 *
 *  \brief Given a 2d image position in the epipolar raster image,
 *  output the 3d ground position with reference to the rectification coordinate system
 *
 *  \param [in] image 2d image position (col, row)
 *  \return Returns 2d rectified ground position (X, Y)
 *
 *  \details 
 */
Point2d EpipolarRect::EpiToRect(const Point2d &epi) const {
  return {0, 0};
}

/**
 *  Rectification to epipolar projection
 *
 *  \brief Given a 2d ground position with reference to the rectification ground coordinate
 * system, output the 2d image position in the epipolar raster image
 *
 *  \param [in] rect 2d rectified ground position (X, Y)
 *  \return Returns 2d image position (col, row)
 *
 *  \details 
 */
Point2d EpipolarRect::RectToEpi(const Point2d &rect) const {
  return { 0, 0 };
}
