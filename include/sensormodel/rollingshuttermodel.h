/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"
#include "sensormodel/utils/imagemodel.h"

#include <array>
#include <chrono>

namespace earthxpert {
class FrameIP;
class ScannerEP;
class SatelliteDP;
class ScannerEPCorr;

/**
 *  The class RollingShutterModel implements the physical geometry model
 *  for optical rolling-shutter frame sensors.
 */
class EXSENSORMODEL_EXPORTS RollingShutterModel final : public SensorModel {
public:
  /**
   *  \brief Construct an aerial rolling-shuter geometry model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship
   *  \param [in] scannerIP intrinsic parameters defining scanner
   * interior orientation
   *  \param [in] scannerEP extrinsic parameters defining
   * scanner exterior orientation
   *  \param [in] groundSRS ground coordinate system
   */
  RollingShutterModel(std::shared_ptr<DigitalImageModelImp> imgModel,
                      std::shared_ptr<FrameIP> scannerIP,
                      std::shared_ptr<ScannerEP> scannerEP,
                      std::shared_ptr<OGRSpatialRef> groundSRS);

  /**
   *  \brief Construct a satellite rolling-shuter geometry model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship \param [in] scannerIP intrinsic parameters defining scanner
   * interior orientation \param [in] scannerEP extrinsic parameters defining
   * scanner exterior orientation \param [in] satelliteDP distortion parameters
   * defining external effects \param [in] groundSRS ground coordinate system
   */
  RollingShutterModel(std::shared_ptr<DigitalImageModelImp> imgModel,
                      std::shared_ptr<FrameIP> scannerIP,
                      std::shared_ptr<ScannerEP> scannerEP,
                      std::shared_ptr<SatelliteDP> satelliteDP,
                      std::shared_ptr<OGRSpatialRef> groundSRS);

  RollingShutterModel(const RollingShutterModel &rhs);
  RollingShutterModel(RollingShutterModel &&rhs) noexcept;

  ~RollingShutterModel() {}

  RollingShutterModel &operator=(const RollingShutterModel &rhs);
  RollingShutterModel &operator=(RollingShutterModel &&rhs) noexcept;

  bool operator==(const RollingShutterModel &rhs) const;
  bool operator!=(const RollingShutterModel &rhs) const;

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

  // access
  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eMT_ROLLING;
  }

  /**
   *  \return Return raster size of image (numCols, numRows) [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return mImgModel->GetImageSize();
  }

  // block adjustment support
  /**
   *  \brief Set correction model in ground space.
   *
   *  \param [in] scannerEPCorr ephemeris & attitude corrections
   */
  void SetCorrection(std::shared_ptr<ScannerEPCorr> scannerEPCorr);

private:
  std::shared_ptr<DigitalImageModelImp>
      mImgModel{}; // image model defining raster vs focal plane relationship
  std::shared_ptr<FrameIP>
      mFrameIP{}; // intrinsic parameters defining frame interior orientation
  std::shared_ptr<ScannerEP> mScannerEP{}; // extrinsic parameters defining
                                           // scanner exterior orientation
  std::shared_ptr<SatelliteDP>
      mSatelliteDP{}; // distortion parameters defining external effects

  std::shared_ptr<ScannerEPCorr>
      mScannerEPCorr{}; // ephemeris & attitude corrections

  bool EqualTo(const ISensorModel &rhs) const override;

  // cached calculation for better performance
  // cv::Mat1d mSensorToBody;      // 3x3 rotation matrix from 3d sensor space
  // to 3d body space
  std::shared_ptr<ScannerEP>
      mFusedScannerEP; // extrinsic parameters combining original extrinsic
                       // parameters and their corrections

  void BuildCubeG2S();
};

using UPRollingShutterModel = std::unique_ptr<RollingShutterModel>;
using SPRollingShutterModel = std::shared_ptr<RollingShutterModel>;
} // namespace earthxpert
