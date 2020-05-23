/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/iimagemodel.h"

namespace earthxpert {
/**
 *  The class defines image product vs frame/focal plane relationship
 *  for the CCD scanned digital imagery.
 *
 *  \detail
 */
class WXSENSORMODEL_EXPORTS DigitalImageModelImp final : public IImageModel {
public:
  /**
   *  Constructor
   *
   *  \param [in] numCols Image width in pixels
   *  \param [in] numRows Image height in pixels
   *  \param [in] pixSizeX Dectector size in X direction [unit: mm/pixel]
   *  \param [in] pixSizeY Dectector size in Y direction
   */
  DigitalImageModelImp(int numCols, int numRows, double pixSizeX, double pixSizeY);

  DigitalImageModelImp(const DigitalImageModelImp &rhs) = default;
  DigitalImageModelImp(DigitalImageModelImp &&rhs) noexcept = default;
  ~DigitalImageModelImp() = default;

  DigitalImageModelImp &operator=(const DigitalImageModelImp &rhs) = default;
  DigitalImageModelImp &operator=(DigitalImageModelImp &&rhs) noexcept = default;

  bool operator==(const DigitalImageModelImp &rhs) const;
  bool operator!=(const DigitalImageModelImp &rhs) const;

  std::unique_ptr<DigitalImageModelImp> Clone() const {
    return std::make_unique<DigitalImageModelImp>(*this);
  }

  /**
   *  \brief Given a 2d position in image space,
   *  output the 2d position in focal plane coordinate system.
   *
   *  \param [in] cr 2d raster position (col, row) in image space [unit: pixel]
   *  \return Return 2d metric position (x, y) in focal plane [unit: mm] if the operation succeeds
   */
  Point2d CRToXY(const Point2d &cr) const override;

  /**
   *  \brief Given a 2d position in focal plane coordinate system,
   *  output the 2d position in image space.
   *
   *  \param [in] xy 2d metric position (x, y) in focal plane [unit: mm]
   *  \return Return 2d raster position (col, row) in image space [unit: pixel] if the operation succeeds
   */
  Point2d XYToCR(const Point2d &xy) const override;

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return { mNumCols, mNumRows };
  }

  /**
   *  \return Return metric size of frame (width, height) [unit: mm]
   */
  std::pair<double, double> GetSensorSize() const noexcept override;

private:
  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product

  double mPixSizeX{};   // pixel size in X-direction on focal plane [unit: mm/pix]
  double mPixSizeY{};   // pixel size in Y-direction on focal plane [unit: mm/pix]

  // cached origin position
  Point2d mOrigin{ 0.0, 0.0 };  // position of PPS in image coordinates [unit: pixel]
  double mPpmmX{ 1.0 };     // number of pixels per mm in X-direction [unit: pix/mm]
  double mPpmmY{ 1.0 };     // number of pixels per mm in Y-direction [unit: pix/mm]
};

/**
 *  The class defines image product vs frame/focal plane relationship
 *  for the scanned film imagery.
 *
 *  \detail
 *  \remark test 6-parameter model (offset + affine)
 */
class WXSENSORMODEL_EXPORTS FilmImageModelImp final : public IImageModel {
public:
  /**
   *  Constructor
   *
   *  \param [in] numCols Image width in pixels
   *  \param [in] numRows Image height in pixels
   *  \param [in] crToX Four coefficients to convert (col, row) in image to x in focal plane
   *  \param [in] crToY crToY Four coefficients to convert (col, row) in image to y in focal plane
   *  \param [in] xyToC xyToC Four coefficients to convert (x, y) in focal plane to col in image
   *  \param [in] xyToR xyToR Four coefficients to convert (x, y) in focal plane to row in image
   */
  FilmImageModelImp(int numCols, int numRows, 
    const std::array<double, 4> &crToX, const std::array<double, 4> &crToY,
    const std::array<double, 4> &xyToC, const std::array<double, 4> &xyToR);

  FilmImageModelImp(const FilmImageModelImp &rhs) = default;
  FilmImageModelImp(FilmImageModelImp &&rhs) noexcept = default;
  ~FilmImageModelImp() = default;

  FilmImageModelImp &operator=(const FilmImageModelImp &rhs) = default;
  FilmImageModelImp &operator=(FilmImageModelImp &&rhs) noexcept = default;

  bool operator==(const FilmImageModelImp &rhs) const;
  bool operator!=(const FilmImageModelImp &rhs) const;

  std::unique_ptr<FilmImageModelImp> Clone() const {
    return std::make_unique<FilmImageModelImp>(*this);
  }

  /**
   *  \brief Given a 2d position in image space,
   *  output the 2d position in focal plane coordinate system.
   *
   *  \param [in] cr 2d raster position (col, row) in image space [unit: pixel]
   *  \return Return 2d metric position (x, y) in focal plane [unit: mm] if the operation succeeds
   */
  Point2d CRToXY(const Point2d &cr) const override;

  /**
   *  \brief Given a 2d position in focal plane coordinate system,
   *  output the 2d position in image space.
   *
   *  \param [in] xy 2d metric position (x, y) in focal plane [unit: mm]
   *  \return Return 2d raster position (col, row) in image space [unit: pixel] if the operation succeeds
   */
  Point2d XYToCR(const Point2d &xy) const override;

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return { mNumCols, mNumRows };
  }

  /**
   *  \return Return metric size of frame (width, height) [unit: mm]
   */
  std::pair<double, double> GetSensorSize() const noexcept override;

private:
  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product

  /**
   * convert raster position (col, row) in image space to x coordinate
   * (unit: millimetre) in focal plane coordinate system.
   *
   * x = CRToX[0] + CRToX[1]*C + CRToX[2]*R + CRToX[3]*C*R
   */
  std::array<double, 4> mCRToX{0.0, 1.0, 0.0, 0.0};
  /**
   * convert raster position (col, row) in image space to y coordinate
   * (unit: millimetre) in focal plane coordinate system.
   *
   * y = CRToY[0] + CRToY[1]*C + CRToY[2]*R + CRToY[3]*C*R
   */
  std::array<double, 4> mCRToY{0.0, 0.0, -1.0, 0.0};
  /**
   * convert focal plane position (x, y) to image column coordinate.
   *
   * C = XYToC[0] + XYToC[1]*x + XYToC[2]*y + XYToC[3]*x*y
   */
  std::array<double, 4> mXYToC{0.0, 1.0, 0.0, 0.0};
  /**
   * convert focal plane position (x, y) to image row coordinate.
   *
   * R = XYToR[0] + XYToR[1]*x + XYToR[2]*y + XYToR[3]*x*y
   */
  std::array<double, 4> mXYToR{0.0, 0.0, -1.0, 0.0};
};
} // namespace watxtract
