/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/pushbroommodel.h"
#include "core/math/interpcube.h"

#include <array>
#include <chrono>

namespace earthxpert {
class ScannerImageModel;
class ScannerIP;
class ScannerEP;
class PushBroomModel;

/**
 *  The class AdsModel implements the physical geometry model
 *  for aerial digital scanner (ADS) line-scanning sensors.
 */
class EXSENSORMODEL_EXPORTS AdsModel final : public SensorModel {
public:
  ~AdsModel() {}

  /**
   *  \brief Construct an aerial pushbroom geometry model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship \param [in] scannerIP intrinsic parameters defining scanner
   * interior orientation \param [in] scannerEP extrinsic parameters defining
   * scanner exterior orientation \param [in] origin origin of the LSR
   * coordinate system \param [in] groundSRS ground coordinate reference system
   */
  AdsModel(std::shared_ptr<ScannerImageModel> imgModel,
           std::shared_ptr<ScannerIP> scannerIP,
           std::shared_ptr<ScannerEP> scannerEP, const Point2d &origin,
           std::shared_ptr<SpatialReferenceSystem> groundSRS);

  AdsModel(const AdsModel &rhs);
  AdsModel(AdsModel &&rhs) noexcept;

  AdsModel &operator=(const AdsModel &rhs);
  AdsModel &operator=(AdsModel &&rhs) noexcept;

  bool operator==(const AdsModel &rhs) const;
  bool operator!=(const AdsModel &rhs) const;

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
   * system \return Returns true if the operation succeeds
   *
   *  \details More details
   */
  bool ImageToGround(const Point2d &image, Point3d &ground) const override;

  /**
   *  \brief Given a 2d position in image coordinate system and a list of
   *  heights, output ground horizontal positions in ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
   * system
   *
   *  \details More details
   */
  size_t ImageToGround(const Point2d &image,
                       std::vector<Point3d> &ground) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the image coordinate system.
   *
   *  \param [in] ground 3d ground position (X, Y, Z)
   *  \param [out] image 2d image position (x, y)
   *  \return Returns true if operation succeeds
   *
   *  \details More details
   */
  bool GroundToImage(const Point3d &ground, Point2d &image) const override;

  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eADS;
  }

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return {0, 0};
  }

  // block adjustment support

private:
  std::unique_ptr<AirPushbroomModel>
      mPbModel; // pushbroom model define in LSR cartetian coordinate system
  Point2d
      mLsrOrigin; // longitude & latitude coordinates defining the origin of LSR

  bool EqualTo(const ISensorModel &rhs) const override;

  // cached calculation for better performance
  std::unique_ptr<InterpCubeD>
      mEll2Lsr; // interpolation cube for col/row/ell_hgt to lsr_hgt transform
};
} // namespace earthxpert
