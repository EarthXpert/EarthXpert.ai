/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/sensormodel.h"
#include "sensormodel/framecp.h"
#include "sensormodel/frameep.h"
#include "sensormodel/frameepcorr.h"
#include "sensormodel/imagemodelpolicy.h"
#include "sensormodel/utils/frameip.h"
#include "sensormodel/utils/frameipcorr.h"

#include <opencv2/core/mat.hpp>

namespace earthxpert {
/**
 *  The class FrameModel implements the projective geometry of
 *  the pin-hole model for the global-shutter frame camera.
 *
 *  \tparam ImageModelPolicy defines the relationship between
 * the image raster coordinate system and the focal plane coordinate system
 */
template <class ImageModelPolicy>
class EXSENSORMODEL_EXPORTS FrameModel final : public SensorModel {
public:
  using PolicyType = ImageModelPolicy;

  // Optical axis direction
  enum OpticalDir {
    eOD_OBLIQUE,   // oblique
    eOD_VERTICAL   // vertical
  };

  /**
   *  Constructor.
   *
   *  \brief Construct a geometry model considering corrections from
   *  various effects, such as atmospheric refraction, earth curvature, etc.
   *
   *  \param [in] imageModel transformation between focal plane and imagery
   *  \param [in] frameIP intrinsic parameters defining interior orientation
   *  \param [in] frameEP extrinsic parameters defining exterior orientation
   *  \param [in] frameCP distortion parameters defining external effects
   */
  FrameModel(ImageModelPolicy &&imageModel, std::shared_ptr<FrameIP> frameIP,
             std::unique_ptr<FrameEP> &&frameEP,
             std::shared_ptr<FrameCP> frameCP);

  /**
   *  \brief Construct a simple pin-hole geometry model.
   *
   *  \param [in] imageModel transformation between focal plane and imagery
   *  \param [in] frameIP intrinsic parameters defining interior orientation
   *  \param [in] frameEP extrinsic parameters defining exterior orientation
   */
  FrameModel(ImageModelPolicy &&imageModel, std::shared_ptr<FrameIP> frameIP,
             std::unique_ptr<FrameEP> &&frameEP);

  FrameModel(const FrameModel &model);
  FrameModel(FrameModel &&model) noexcept;
  ~FrameModel() = default;

  FrameModel &operator=(const FrameModel &model);
  FrameModel &operator=(FrameModel &&model) noexcept;

  bool operator==(const FrameModel &model) const;
  bool operator!=(const FrameModel &model) const;

  std::unique_ptr<ISensorModel> Clone() const override {
    return std::make_unique<FrameModel>(*this);
  }

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   * [pixel] \param [in/out] ground 3d ground position (X, Y, Z) in ground
   * coordinate system \return Returns true if the operation succeeds
   *
   *  \details
   */
  bool ImageToGround(const Point2d &image, Point3d &ground) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the image coordinate system.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] image 2d position (x, y) in imagery raster coordinate system
   * [pixel] \return Returns true if operation succeeds
   *
   *  \details
   */
  bool GroundToImage(const Point3d &ground, Point2d &image) const override;

  /**
   * \return Return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eMT_FRAME;
  }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return pointer to a coordinate system
   */
  std::shared_ptr<SpatialReferenceSystem> GetCoordinateSystem() const
      noexcept override;

  /**
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override {
    return eST_MAP;
  }

  std::pair<int, int> GetImageSize() const noexcept override;

  /**
   *  \return Returns the middle height
   */
  double GetMeanHeight() const noexcept override {
    return 0.0;
  }

  // block adjustment helpers

private:
  friend class FrameModelMarshaller;
  template<typename> friend class FrameEpipolarModel;
  friend class FrameStereoModel;

  static constexpr int kMaxIterG2I{ 10 };
  static constexpr double kTolFP{ 0.0001 }; // 0.1 um on focal plane

  ImageModelPolicy mImageModel; // model defining the transformation between
                                // focal plane and imagery
  std::shared_ptr<FrameIP>
      mFrameIP{}; // intrinsic parameters defining interior orientation
  std::unique_ptr<FrameEP>
      mFrameEP{}; // extrinsic parameters defining exterior orientation
  std::shared_ptr<FrameCP>
      mFrameCP{}; // correction parameters defining external effects
  OpticalDir mOpticalDir{ eOD_VERTICAL };

  std::shared_ptr<FrameIPCorr> mFrameIPCorr{}; // interior orienttion corrections
  std::shared_ptr<FrameEPCorr> mFrameEPCorr{}; // exterior orienttion corrections

  void CheckModel() const;
  bool EqualTo(const ISensorModel &rhs) const override;

  /**
   *  Sensor-to-ground forward projection.
   *
   *  \brief Given a 2d position in focal plane coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in sensor focal plane coordinate
   * system [mm]
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground
   * coordinate system
   *  \return Return true if the operation succeeds
   *
   *  \details
   */
  bool SensorToGround(const Point2d &sensor, Point3d &ground) const;

  /**
   *  Ground-to-sensor backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the focal plane coordinate system.
   *
   *  \param [in] ground 3d ground position (X, Y, Z)
   *  \param [out] sensor 2d sensor position (x, y) [mm]
   *  \return Return true if operation succeeds
   *
   *  \details
   */
  bool GroundToSensor(const Point3d &ground, Point2d &sensor) const;

  /**
   *  Actual-to-apparant convertion.
   *
   *  \brief Convert the measured actual position to the idea apparant position
   * by canceling corrects for various distortions in focal plane, including
   * radial/decentering optical distortions, grid distortion, atmospheric
   * refraction, and/or earth curvature.
   *
   *  \param [in] sensorAct the measured position where the ray actually hits
   * the focal plane
   *  \param [in] height the ground height above ellipsoid [m]
   *  \param [in] sensorApp the ideal position where the ray wpould apparantly
   * hit the focal plane if existing no distortions \return Return true if the
   * operation succeeds
   */
  bool SensorActualToApparant(const Point2d& sensorAct, const double height,
    Point2d& sensorApp) const;

  Point2d SensorActualToApparantPPS(const Point2d& sensor) const;
  Point2d SensorActualToApparantPPA(const Point2d& sensor, double height) const;
  Point2d SensorActualToApparantNP(const Point2d& sensor, double height) const;

  /**
   *  Apparant-to-actual convertion.
   *
   *  \brief Convert the idea apparant position to the measured actual position
   * by inserting corrects for various distortions in focal plane, including
   * radial/decentering optical distortions, grid distortion, atmospheric
   * refraction, and/or earth curvature.
   *
   *  \param [in] sensorApp the ideal position where the ray wpould apparantly
   * hit the focal plane if existing no distortions
   *  \param [in] height the ground height above ellipsoid [m]
   *  \param [in] sensorAct the measured position where the ray actually
   * hits the focal plane
   *  \return Return true if the operation succeeds
   */
  bool SensorApparantToActual(const Point2d& sensorApp, double height,
    Point2d& sensorAct) const;

  Point2d SensorApparantToActualNP(const Point2d& sensor, double height) const;
  Point2d SensorApparantToActualPPA(const Point2d& sensor, double height) const;
  Point2d SensorApparantToActualPPS(const Point2d& sensor) const;

  void CopyFrom(const FrameModel &model);
  void MoveFrom(FrameModel &&model) noexcept;
  void Clear() noexcept;

  // cached calculation for better performance
  double mInvF2{};   // reciprocal of focal length squared
  double mCurvFactor{}; // factor for earth curvature correction
  Point3d mSensorCenter{}; // fused 3d position of rear node center in ground
                           // coordinate system
  cv::Mat1d mG2S{cv::Mat1d::eye(3, 3)}; // fused 3x3 rotation matrix from 
                                        // 3d ground space to 3d sensor space

  void CalcFusedEO();
};

/**
 *  \brief Frame camera has only two acquisition approaches: film and digital.
 */
using DigitalFrameModel = FrameModel<DigitalImageModel>;
using FilmFrameModel = FrameModel<FilmImageModel>;

using UPDigitalFrameModel = std::unique_ptr<DigitalFrameModel>;
using SPDigitalFrameModel = std::shared_ptr<DigitalFrameModel>;

using UPFilmFrameModel = std::unique_ptr<FilmFrameModel>;
using SPFilmFrameModel = std::shared_ptr<FilmFrameModel>;
} // namespace earthxpert
