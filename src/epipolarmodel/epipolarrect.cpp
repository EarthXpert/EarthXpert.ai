/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarrect.h"
#include "core/utils/typehelper.h"

#include <cmath>
#include <limits>

using namespace earthxpert;

/**
 *  Constructor.
 *
 *  \param [in] origin Origin of epi coordinate system
 *  \param [in] height Rectification height
 *  \param [in] scale Rectification scale
 *  \param [in] rotation Rectification rotation (unit: rad)
 *  \param [in] offset Rectification XY-offset
 *  \param [in] numCols Number of cols of epipolar image
 *  \param [in] numRows Number of rows of epipolar image
 */
EpipolarRect::EpipolarRect(const Point3d origin, const double height, 
  const double scale, const double rotation, const Point2d& offset, 
  const int numCols, const int numRows)
    : mOrigin(origin), mHeight(height), mScale(scale), mRotation(rotation), 
      mOffset(offset), mNumCols(numCols), mNumRows(numRows) {
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

    if (!Eq(mOffset.x, rhs.mOffset.x) || !Eq(mOffset.y, rhs.mOffset.y))
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
 */
Point2d EpipolarRect::EpiToRect(const Point2d &epi) const {
  const Point2d e{ mOffset.x + epi.x, mOffset.y + mNumRows - epi.y };
  const Point2d rect{ mCosRot * e.x - mSinRot * e.y, 
                      mSinRot * e.x + mCosRot * e.y };
  return rect * mInvScale;
}

/**
 *  Rectification to epipolar projection
 *
 *  \brief Given a 2d ground position with reference to the rectification ground coordinate
 * system, output the 2d image position in the epipolar raster image
 *
 *  \param [in] rect 2d rectified ground position (X, Y)
 *  \return Returns 2d image position (col, row)
 */
Point2d EpipolarRect::RectToEpi(const Point2d &rect) const {
  const Point2d r{ mCosRot * rect.x + mSinRot * rect.y,
                   - mSinRot * rect.x + mCosRot * rect.y };
  return { r.x * mScale - mOffset.x, mNumRows - r.y * mScale + mOffset.y };
}
