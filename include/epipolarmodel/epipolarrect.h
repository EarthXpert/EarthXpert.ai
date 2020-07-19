/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/exepipolarmodel.h"
#include "3rdparty/opencv/exopencv.h"

namespace earthxpert {
/**
 *  The class EpipolarRect defines similarity transform between ground
 * coordinate system of sensor model and raster coordinate system of epipolar
 * model.
 */
class EXEPIPOLARMODEL_EXPORTS EpipolarRect {
public:
  EpipolarRect(Point3d origin, double height, double scale, double rotation, 
    const Point2d& offset, int numCols, int numRows);
  EpipolarRect(const EpipolarRect &rhs) = default;
  EpipolarRect(EpipolarRect &&rhs) = default;
  ~EpipolarRect() = default;

  EpipolarRect &operator=(const EpipolarRect &rhs) = default;
  EpipolarRect &operator=(EpipolarRect &&rhs) = default;

  bool operator==(const EpipolarRect &rhs) const;
  bool operator!=(const EpipolarRect &rhs) const;

  /**
   *  Epipolar to rectified projection
   *
   *  \brief Given a 2d image position in the epipolar coordinate system,
   *      output the 2d ground position with reference to the rectification ground coordinate
   * system
   *
   *  \param [in] image 2d image position (col, row)
   *  \return Returns 2d rectified ground position (X, Y)
   */
  Point2d EpiToRect(const Point2d &epi) const;

  /**
   *  Rectification to epipolar projection
   *
   *  \brief Given a 2d ground position with reference to the rectification ground coordinate
   * system, output the 2d image position in the epipolar raster image
   *
   *  \param [in] rect 2d rectified ground position (Long, Lat, Height)
   *  \return Returns 2d image position (col, row)
   */
  Point2d RectToEpi(const Point2d &rect) const;

  Point3d GetOrigin() const noexcept { return mOrigin; }
  double GetHeight() const noexcept { return mHeight; }
  double GetScale() const noexcept { return mScale; }
  double GetRotation() const noexcept { return mRotation; }
  Point2d GetOffset() const noexcept { return mOffset; }
  int GetNumCols() const noexcept { return mNumCols; }
  int GetNumRows() const noexcept { return mNumRows; }

private:
  template<typename> friend class EpipolarModel;

  Point3d mOrigin{0, 0, 0};     // Origin of epi coordinate system
  double mHeight{0.0};   // Rectification height [meter]
  double mScale{1.0};    // Rectification scale: metric -> pixel
  double mRotation{0.0}; // Rectification rotation [rad]
  Point2d mOffset{};     // Rectification XY-offset [pixel]
  int mNumCols{ 0 };       // Number of cols of epipolar image
  int mNumRows{ 0 };       // Number of rows of epipolar image

  // cached data for better perforamnce
  double mInvScale{1.0};    // GSD: pixel -> 
  double mCosRot{};
  double mSinRot{};
};
} // namespace earthxpert
