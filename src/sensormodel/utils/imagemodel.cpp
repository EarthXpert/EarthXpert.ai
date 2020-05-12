/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/imagemodel.h"
#include "core/utils/typehelper.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

using watxtract::EqualD;
using watxtract::DigitalImageModelImp;
using watxtract::FilmImageModelImp;
using watxtract::Point2d;
using watxtract::Point3d;

/**
 *  Constructor
 *
 *  \param [in] numCols Image width in pixels
 *  \param [in] numRows Image height in pixels
 *  \param [in] pixSizeX Dectector size in X direction [unit: mm/pixel]
 *  \param [in] pixSizeY Dectector size in Y direction
 */
DigitalImageModelImp::DigitalImageModelImp(const int numCols, const int numRows,
                       const double pixSizeX, const double pixSizeY)
    : mNumCols(numCols), mNumRows(numRows), mPixSizeX(pixSizeX), mPixSizeY(pixSizeY) {
  // Check for image dimension
  if (mNumCols <= 0 || mNumRows <= 0) {
    throw std::invalid_argument("Image dimension must be positive");
  }

  constexpr auto tol{0.001}; // [unit: um]

  if (mPixSizeX < tol) {
    throw std::invalid_argument("Pixel size in X-direction is too small");
  }

  if (mPixSizeY < tol) {
    throw std::invalid_argument("Pixel size in Y-direction is too small");
  }

  // calculate origin and reciprocals
  mOrigin = { mNumCols / 2.0, mNumRows / 2.0 };
  mPpmmX = 1.0 / mPixSizeX;
  mPpmmY = 1.0 / mPixSizeY;
}

bool DigitalImageModelImp::operator==(const DigitalImageModelImp &rhs) const {
  if (mNumCols != rhs.mNumCols || mNumRows != rhs.mNumRows)
    return false;

  const EqualD equalD;
  if (!equalD(mPixSizeX, rhs.mPixSizeX) || !equalD(mPixSizeY, rhs.mPixSizeY))
    return false;

  return true;
}

bool DigitalImageModelImp::operator!=(const DigitalImageModelImp &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 2d position in image space,
 *  output the 2d position in focal plane coordinate system.
 *
 *  \param [in] cr 2d raster position (col, row) in image space [unit: pixel]
 *  \return Return 2d metric position (x, y) in focal plane [unit: mm]
 */
Point2d DigitalImageModelImp::CRToXY(const Point2d &cr) const {
  return { mPixSizeX * (cr.x - mOrigin.x),
           mPixSizeY * (mOrigin.y - cr.y) };
}

/**
 *  \brief Given a 2d position in focal plane coordinate system,
 *  output the 2d position in image space.
 *
 *  \param [in] xy 2d metric position (x, y) in focal plane [unit: mm]
 *  \return Return 2d raster position (col, row) in image space [unit: pixel]
 */
Point2d DigitalImageModelImp::XYToCR(const Point2d &xy) const {
  return { mOrigin.x + mPpmmX * xy.x,
           mOrigin.y - mPpmmY * xy.y };
}

/**
 *  \return Return metric size of frame (width, height) [unit: mm]
 */
std::pair<double, double> DigitalImageModelImp::GetSensorSize() const noexcept {
  return { mPixSizeX * mNumCols, mPixSizeY * mNumRows };
}

/**
 *  Constructor
 *
 *  \param [in] numCols Image width in pixels
 *  \param [in] numRows Image height in pixels
 *  \param [in] crToX Four coefficients to convert (col, row) in image to x in focal plane
 *  \param [in] crToY Four coefficients to convert (col, row) in image to y in focal plane
 *  \param [in] xyToC Four coefficients to convert (x, y) in focal plane to col in image
 *  \param [in] xyToR Four coefficients to convert (x, y) in focal plane to row in image
 */
FilmImageModelImp::FilmImageModelImp(const int numCols, const int numRows,
                       const std::array<double, 4> &crToX,
                       const std::array<double, 4> &crToY,
                       const std::array<double, 4> &xyToC,
                       const std::array<double, 4> &xyToR)
    : mNumCols(numCols), mNumRows(numRows), mCRToX(crToX), mCRToY(crToY),
      mXYToC(xyToC), mXYToR(xyToR) {
  // Check for image dimension
  if (mNumCols <= 0 || mNumRows <= 0) {
    throw std::invalid_argument("Image dimension must be positive");
  }

  constexpr auto tol = std::numeric_limits<double>::epsilon();

  // Check for singular transformation coefficients.
  const auto cr2XY = mCRToX[1] * mCRToY[2] - mCRToX[2] * mCRToY[1];
  if (fabs(cr2XY) < tol) {
    throw std::underflow_error("Image-To-FocalPlane trnasformation is singular");
  }

  const auto xy2CR = mXYToC[1] * mXYToR[2] - mXYToC[2] * mXYToR[1];
  if (fabs(xy2CR) < tol) {
    throw std::underflow_error("FocalPlane-To-Image trnasformation is singular");
  }
}

bool FilmImageModelImp::operator==(const FilmImageModelImp &rhs) const {
  if (mNumCols != rhs.mNumCols || mNumRows != rhs.mNumRows)
    return false;

  const EqualD equalD;

  if (!std::equal(std::cbegin(mCRToX), std::cend(mCRToX),
                  std::cbegin(rhs.mCRToX), equalD))
    return false;
  if (!std::equal(std::cbegin(mCRToY), std::cend(mCRToY),
                  std::cbegin(rhs.mCRToY), equalD))
    return false;
  if (!std::equal(std::cbegin(mXYToC), std::cend(mXYToC),
                  std::cbegin(rhs.mXYToC), equalD))
    return false;
  if (!std::equal(std::cbegin(mXYToR), std::cend(mXYToR),
                  std::cbegin(rhs.mXYToR), equalD))
    return false;

  return true;
}

bool FilmImageModelImp::operator!=(const FilmImageModelImp &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 2d position in image space,
 *  output the 2d position in focal plane coordinate system.
 *
 *  \param [in] cr 2d raster position (col, row) in image space [unit: pixel]
 *  \return Return 2d metric position (x, y) in focal plane [unit: mm]
 */
Point2d FilmImageModelImp::CRToXY(const Point2d &cr) const {
  const auto C{cr.x};
  const auto R{cr.y};
  const auto CR{C * R};

  return { mCRToX[0] + mCRToX[1] * C + mCRToX[2] * R + mCRToX[3] * CR,
           mCRToY[0] + mCRToY[1] * C + mCRToY[2] * R + mCRToY[3] * CR };
}

/**
 *  \brief Given a 2d position in focal plane coordinate system,
 *  output the 2d position in image space.
 *
 *  \param [in] xy 2d metric position (x, y) in focal plane [unit: mm]
 *  \return Return 2d raster position (col, row) in image space [unit: pixel]
 */
Point2d FilmImageModelImp::XYToCR(const Point2d &xy) const {
  const auto XY{xy.x * xy.y};
  return { mXYToC[0] + mXYToC[1] * xy.x + mXYToC[2] * xy.y + mXYToC[3] * XY,
          mXYToR[0] + mXYToR[1] * xy.x + mXYToR[2] * xy.y + mXYToR[3] * XY };
}

/**
 *  \return Return metric size of frame (width, height) [unit: mm]
 */
std::pair<double, double> FilmImageModelImp::GetSensorSize() const noexcept {
  const auto ul = CRToXY(Point2d{ 0.0, 0.0 });
  const auto lr = CRToXY(Point2d{ double(mNumCols), double(mNumRows) });

  const double width = fabs(lr.x - ul.x);
  const double height = fabs(ul.y - lr.y);
  return { width, height };
}
