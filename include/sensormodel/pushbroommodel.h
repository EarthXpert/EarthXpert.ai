/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"
#include "sensormodel/craftpolicy.h"
#include "sensormodel/rationalmodel.h"
#include "sensormodel/scannerep.h"
#include "sensormodel/scannerepcorr.h"
#include "sensormodel/scannerimagemodel.h"
#include "sensormodel/scannerip.h"
#include "sensormodel/utils/linetime.h"
#include "sensormodel/utils/satellitedp.h"

#include <array>
#include <chrono>

namespace earthxpert {
/**
 *  The class PushbroomModel implements the physical geometry model
 *  for optical line-scanning/framelet sensors.
 *
 *  \tparam CraftPolicy defnies behaviours specific to airborne or spacecraft.
 */
template <typename CraftPolicy>
class EXSENSORMODEL_EXPORTS PushbroomModel final : public SensorModel {
public:
  enum CcdUnit {
    eCU_ANGLE,     // line of sight degree
    eCU_METRIC,    // metric mm
    eCU_PIXEL      // non-metric pixel
  };

  /**
   *  \brief Construct an aerial pushbroom geometry model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship 
   *  \param [in] scannerIP intrinsic parameters defining scanner
   * interior orientation 
   *  \param [in] scannerEP extrinsic parameters defining
   * scanner exterior orientation 
   *  \param [in] groundSRS ground coordinate system
   */
  PushbroomModel(std::shared_ptr<ScannerImageModel> imgModel,
                 std::unique_ptr<ScannerIP> &&scannerIP,
                 //LineTime lineTime,
                 std::shared_ptr<ScannerEP> scannerEP,
                 std::shared_ptr<OGRSpatialRef> groundSRS);

  /**
   *  \brief Construct a satellite pushbroom geometry model.
   *
   *  \param [in] imgModel image model defining raster vs focal plane
   * relationship 
   *  \param [in] scannerIP intrinsic parameters defining scanner
   * interior orientation 
   *  \param [in] scannerEP extrinsic parameters defining
   * scanner exterior orientation 
   *  \param [in] satelliteDP distortion parameters defining external effects 
   *  \param [in] groundSRS ground coordinate system
   */
  PushbroomModel(std::shared_ptr<ScannerImageModel> imgModel,
                 std::unique_ptr<ScannerIP> &&scannerIP,
                 //LineTime lineTime,
                 std::shared_ptr<ScannerEP> scannerEP,
                 std::shared_ptr<SatelliteDP> satelliteDP,
                 std::shared_ptr<OGRSpatialRef> groundSRS);

  PushbroomModel(const PushbroomModel &rhs);
  PushbroomModel(PushbroomModel &&rhs) noexcept;

  ~PushbroomModel() {}

  PushbroomModel &operator=(const PushbroomModel &rhs);
  PushbroomModel &operator=(PushbroomModel &&rhs) noexcept;

  bool operator==(const PushbroomModel &rhs) const;
  bool operator!=(const PushbroomModel &rhs) const;

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
  bool GroundToImage(const Point3d& ground, Point2d& image) const override;
  bool GroundToImageNR(const Point3d& ground, Point2d& image) const;

  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eMT_PUSHBROOM;
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
  static constexpr int kMaxIterS2G = 10; // max. number of iterations allowed for SensorToGround
  static constexpr double kCR = 1.0e-3; // step used to calculate numerical difference for col/row
  static constexpr double kFacCR = 0.5 / kCR;
  static constexpr double kTol = std::numeric_limits<double>::epsilon();
  static constexpr double kTolCR = kTolPIX3; // 
  static constexpr double kTol_deg = kTolDEG; // tolerance in unit of degrees

  std::shared_ptr<ScannerImageModel>
      mImgModel{}; // image model defining raster vs focal plane relationship
  std::unique_ptr<ScannerIP> mScannerIP{}; // intrinsic parameters defining
                                           // scanner interior orientation
  // LineTime mLineTime;
  std::shared_ptr<ScannerEP> mScannerEP{}; // extrinsic parameters defining
                                           // scanner exterior orientation
  std::shared_ptr<SatelliteDP>
      mSatelliteDP{}; // distortion parameters defining external effects

  std::shared_ptr<ScannerEPCorr> mScannerEPCorr{}; // ephemeris & attitude corrections

  bool EqualTo(const ISensorModel &rhs) const override;

  // cached calculation for better performance
  // cv::Mat1d mSensorToBody;      // 3x3 rotation matrix from 3d sensor space
  // to 3d body space
  std::shared_ptr<ScannerEP>
      mFusedScannerEP; // extrinsic parameters combining original extrinsic
                       // parameters and their corrections

  // interpolation cube to provide approximate image coordinates for GroundToImage()
  std::unique_ptr<RationalModel> mCubeG2I;   
  void BuildCubeG2I();
};

using AirPushbroomModel = PushbroomModel<AircraftPolicy>;
using SpacePushbroomModel = PushbroomModel<SpacecraftPolicy>;
} // namespace earthxpert
