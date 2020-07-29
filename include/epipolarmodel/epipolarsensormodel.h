/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"

#include <memory>
#include <utility>

namespace earthxpert {
/**
 *  The class EpipolarSensorModel defines the template algorithms
 *  for epipolar images generated from image products with sensor models.
 */
class EXEPIPOLARMODEL_EXPORTS EpipolarSensorModel 
  : public IEpipolarModel, public ISensorModel{
public:
  /**
   *  \param [in] sensorModel sensor model associated with the image
   *  \param [in] pos Component position of stereo model
   *  \param [in] epiBound epipolar boundary of valid region
   */
  EpipolarSensorModel(std::shared_ptr<ISensorModel> sensorModel, 
    StereoPos pos, const Polygon2d& epiBound);
  virtual ~EpipolarSensorModel() = default;

  /**
   *  Epipolar-to-raw transform.
   *
   *  \brief Given a 2d poistion on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in/out] raw 2d position (x, y) in image raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool EpiToRaw(const Point2d& epi, Point2d& raw) const override;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster coordinate
   * system
   *  \param [in] raw List of 2d positions (x, y) in image raster
   * coordinate system
   *  \return Returns true if operation succeeds
   */
  size_t EpiToRaw(const std::vector<Point2d>& epi, std::vector<Point2d>& raw) const override;

  /**
   *  Raw-to-epipolar transform.
   *
   *  \brief Given a 2d poistion on image, output the corresponding position on
   * epipolar.
   *
   *  \param [in] raw 2d position (x, y) in image raster coordinate system
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool RawToEpi(const Point2d& raw, Point2d& epi) const override;

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] raw List of 2d positions (x, y) in image raster coordinate
   * system
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster
   * coordinate system
   *  \return Returns true if operation succeeds
   */
  size_t RawToEpi(const std::vector<Point2d>& raw, std::vector<Point2d>& epi) const override;

  /**
   *  \return Returns the internal sensor model
   */
  virtual const ISensorModel* GetSensorModel() const noexcept override {
    return mSensorModel.get();
  }

  const Polygon2d& GetBoundary() const noexcept override {
    return mBound;
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

  std::unique_ptr<ISensorModel> Clone() const override {
    return mSensorModel->Clone();
  }

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in epipolar image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (x, y) in epipolar imagery raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate
   * system \return Returns true if the operation succeeds
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
   *  \param [in/out] ground 3d ground positions (X, Y, Z) in ground coordinate
   * system
   */
  size_t ImageToGround(const Point2d& image, std::vector<Point3d>& ground) const override;

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
   *  \param [out] image 2d position (x, y) in epipolar imagery raster coordinate system
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
   *  \return Returns raster columns & rows of raw image [unit: pixel]
   */
  std::pair<int, int> GetRawImageSize() const noexcept {
    return mSensorModel->GetImageSize();
  }

  /**
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override { return mSensorModel->GetSrsType(); }

  /**
   *  \return Returns the middle height
   */
  virtual double GetMeanHeight() const noexcept override { return mSensorModel->GetMeanHeight(); }

protected:
  std::shared_ptr<ISensorModel> mSensorModel; // image model composing of the stereo model
  StereoPos mPos{ eIGNORE };    // Component position of stereo model
  Polygon2d mBound;   // boundary pixels of valid region w.r.t. the center of UL corner

private:
  void Initialize();

  // cached data for convenience
};
} // namespace earthxpert
