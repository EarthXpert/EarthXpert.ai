/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"

namespace earthxpert {
/**
 *  The class SensorEpipolarModel defines the template algorithms
 *  for epipolar images generated from image products with sensor models.
 *  For the ortho-ready product with a sensor model, the epipolar image itslef
 *  is treated as euqivelant to the raw image, thus providing the identity
 *  conversion when implementing the interface IEpipolarModel.
 */
class EXEPIPOLARMODEL_EXPORTS SensorEpipolarModel 
  : public IEpipolarModel, public ISensorModel {
public:
  /**
   *  \param [in] pos Component position of stereo model
   *  \param [in] sensorModel sensor model associated with the image
   *  \param [in] epiBound epipolar boundary of valid region
   */
  SensorEpipolarModel(StereoPos pos, 
    SPISensorModel sensorModel, const Polygon2d& epiBound);
  virtual ~SensorEpipolarModel() = default;

  /**
   *  Epipolar-to-raw transform.
   *
   *  \brief Given a 2d poistion on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (u, v) in epipolar image pixel coordinate system
   *  \param [out] raw 2d position (x, y) in image raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool EpiToRaw(const Point2d& epi, Point2d& raw) const override {
    raw = epi;
    return true;
  }

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster CS
   *  \param [in] raw List of 2d positions (x, y) in image raster CS
   *  \return Returns true if operation succeeds
   */
  size_t EpiToRaw(const std::vector<Point2d>& epi, std::vector<Point2d>& raw) const override;

  /**
   *  Raw-to-epipolar transform.
   *
   *  \brief Given a 2d poistion on image, output the corresponding position on
   * epipolar.
   *
   *  \param [in] raw 2d position (u, v) in image raster coordinate system
   *  \param [out] epi 2d position (x, y) in epipolar image pixel coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool RawToEpi(const Point2d& raw, Point2d& epi) const override {
    epi = raw;
    return true;
  }

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] raw List of 2d positions (x, y) in image raster CS
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster CS
   *  \return Returns true if operation succeeds
   */
  size_t RawToEpi(const std::vector<Point2d>& raw, 
    std::vector<Point2d>& epi) const override;

  /**
   *  \return Returns algorithm of epipolar model
   */
  virtual EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return eSENSOR;
  }

  /**
   *  \return Returns position in epipolar model
   */
  StereoPos GetStereoPos() const noexcept override { return mPos; }

  /**
   *  \return Returns raster columns & rows of epipolar image [unit: pixel]
   */
  virtual std::pair<int, int> GetImageSize() const noexcept override {
    return mSensorModel->GetImageSize();
  }

  /**
   *  \return Returns the internal sensor model
   */
  virtual const ISensorModel* GetSensorModel() const noexcept override {
    return mSensorModel.get();
  }

  /**
   *  \return Returns the footprint in rectification CS
   */
  const Polygon2d& GetBoundary() const noexcept override { return mBound; }

  /**
   *  \return Returns raster columns & rows of raw image [unit: pixel]
   */
  std::pair<int, int> GetRawImageSize() const noexcept {
    return mSensorModel->GetImageSize();
  }

  /**
   *  \brief compare the equality of two models.
   *
   *  \param [in] rhs the other model to compare with
   *  \return Return true if two models represent same geometry.
   */
  bool operator==(const ISensorModel& rhs) const override {
    return mSensorModel->operator==(rhs);
  }

  /**
   *  \param [in] rhs the other model to compare with
   *  \return Return true if two models represent different geometry.
   */
  bool operator!=(const ISensorModel& rhs) const override {
    return mSensorModel->operator!=(rhs);
  }

 UPISensorModel Clone() const override {
    return mSensorModel->Clone();
  }

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in epipolar image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in epipolar raster CS
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground CS 
   *  \return Returns true if the operation succeeds
   */
  bool ImageToGround(const Point2d& image, Point3d& ground) const override {
    Point2d raw;
    EpiToRaw(image, raw);
    return mSensorModel->ImageToGround(raw, ground);
  }

  bool operator()(const Point2d& image, Point3d& ground) const override {
    return ImageToGround(image, ground);
  }

  /**
   *  \brief Given a 2d position in epipolar image coordinate system and a list of
   *  heights, output ground horizontal positions in ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in imagery raster coordinate system
   *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground CS
   */
  size_t ImageToGround(const Point2d& image, 
    std::vector<Point3d>& ground) const override;

  /**
   *  \brief Given a list of 2d positions in image coordinate system and a
   * corresponding list of heights, output the ground horizontal positions in
   * ground coordinate system.
   *
   *  \param [in] image List of 2d positions (x, y) on image
   *  \param [in/out] ground List of 3d ground positions (X, Y, Z)
   */
  size_t ImageToGround(const std::vector<Point2d>& image,
    std::vector<Point3d>& ground) const override;

  /**
   *  Ground-to-image backward projection.
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system, output the 2d image position in the epipolar image coordinate system.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] image 2d position (x, y) in epipolar raster CS
   *  \return Returns true if operation succeeds
   */
  bool GroundToImage(const Point3d& ground, Point2d& image) const override {
    Point2d raw;
    mSensorModel->GroundToImage(ground, raw);
    return RawToEpi(raw, image);
  }

  bool operator()(const Point3d& ground, Point2d& image) const override {
    return GroundToImage(ground, image);
  }

  /**
   *  \brief Given a list of 3d positions in ground coordinate system,
   *  output a list of 2d sensor positions in image coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] image List of 2d positions (x, y) on image
   */
  size_t GroundToImage(const std::vector<Point3d>& ground,
    std::vector<Point2d>& image) const override;

  /**
   * \return Returns type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return mSensorModel->GetModelType();
  }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return Returns pointer to a coordinate system
   */
  std::shared_ptr<SpatialReferenceSystem>
    GetCoordinateSystem() const noexcept override {
    return mSensorModel->GetCoordinateSystem();
  }

  /**
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override { 
    return mSensorModel->GetSrsType(); 
  }

  /**
   *  \return Returns the middle height
   */
  virtual double GetMeanHeight() const noexcept override { 
    return mSensorModel->GetMeanHeight(); 
  }

protected:
  StereoPos mPos{ eIGNORE };    // Component position of stereo model
  SPISensorModel mSensorModel; // image model composing of the stereo model
  Polygon2d mBound;   // boundary pixels of valid region w.r.t. the center of UL corner

private:
  void Initialize();

  // cached data for convenience
};

using UPSensorEpipolarModel = std::unique_ptr<SensorEpipolarModel>;
using SPSensorEpipolarModel = std::shared_ptr<SensorEpipolarModel>;

// typically for satellite remote sensing (ref. epipolarmodel.h)
using OrthoReadyModel = SensorEpipolarModel;
using UPOrthoReadyModel = std::unique_ptr<OrthoReadyModel>;
using SPOrthoReadyModel = std::shared_ptr<OrthoReadyModel>;
} // namespace earthxpert
