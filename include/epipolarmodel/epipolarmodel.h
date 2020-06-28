/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"
#include "sensormodel/isensormodel.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/math/interpgrid.h"
#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/groundpolicy.h"

#include <optional>

namespace earthxpert {

/**
 *  The class EpipolarModel defines a sensor-independent algorithm
 *  for generating epipolar images from image products with sensor models.
 *
 *  \param GroundPolicy defines the behaviour of and relationshp between
 * the model coordinate system and the rectification coordinate system
 */
template <class GroundPolicy>
class EXEPIPOLARMODEL_EXPORTS EpipolarModel final 
  : public IEpipolarModel, public ISensorModel {
public:
  using PolicyType = GroundPolicy;

  /**
   *  Constructor.
   *
   *  \param [in] sensorModel sensor model associated with the image
   *  \param [in] epiRect epipolar rectification defining the transform 
   *              between the epipolar model coordinate system and epipolar 
   *              raster coordinate system for the image.
   *  \param [in] pos Component position of stereo model
   *  \param [in] groundPolicy ground policy
   */
  EpipolarModel(std::shared_ptr<ISensorModel> sensorModel,
                std::unique_ptr<EpipolarRect> &&epiRect, StereoPos pos,
                std::shared_ptr<GroundPolicy> groundPolicy);
  ~EpipolarModel() = default;

  /**
   *  Epipolar-to-raw transform.
   *
   *  \brief Given a 2d poistion on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
   *  \param [in] raw 2d position (col, row) in image raster coordinate system
   *  \return Returns true if operation succeeds
   */
  bool EpiToRaw(const Point2d &epi, Point2d &raw) const override;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (col, row) in epipolar raster coordinate
   * system 
   *  \param [in] raw List of 2d positions (col, row) in image raster
   * coordinate system 
   *  \return Returns number of successful operations
   */
  size_t EpiToRaw(const std::vector<Point2d> &epi, std::vector<Point2d> &raw) const override;

  /**
   *  Raw-to-epipolar transform.
   *
   *  \brief Given a 2d poistion on image, output the corresponding position on
   * epipolar.
   *
   *  \param [in] raw 2d position (col, row) in image raster coordinate system
   *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
   *  \return Returns true if operation succeeds
   */
  bool RawToEpi(const Point2d &raw, Point2d &epi) const override;

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] raw List of 2d positions (col, row) in image raster coordinate
   * system
   *  \param [in] epi List of 2d positions (col, row) in epipolar raster
   * coordinate system
   *  \return Returns number of successful operations
   */
  size_t RawToEpi(const std::vector<Point2d> &raw, std::vector<Point2d> &epi) const override;

  /**
   *  \return Returns algorithm of epipolar model
   */
  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return ePLANERECT;
  }

  const EpipolarRect *GetEpiRect(StereoPos pos) const noexcept {
    return mEpiRect.get();
  }

  Point3d GetOrigin() const noexcept { return mOrigin; }

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
   *  \return Returns type of SRS
   */
  SrsType GetSrsType() const noexcept override { return mSensorModel->GetSrsType(); }

  /**
   *  \return Returns raster columns & rows [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
   return mSensorModel->GetImageSize();
  }

  /**
   *  \return Returns the middle height
   */
  double GetMeanHeight() const noexcept override { return mSensorModel->GetMeanHeight(); }


private:
  static constexpr int kMaxIters{ 5 };
  static constexpr double kHgtTol{ 0.0001 }; // Height tolerance [0.1mm]?
  static constexpr int kGridStride{ 256 };

  std::shared_ptr<ISensorModel> mSensorModel; // image composing of the stereo model
  std::unique_ptr<EpipolarRect> mEpiRect;     // epipolar rectification
  StereoPos mPos{ eIGNORE };    // Component position of stereo model
  std::shared_ptr<GroundPolicy> mGroundPolicy;

  /**
   *  Raw-to-rect transform.
   *
   *  \brief Given a 2d poistion on raw image, output the 2d position in
   * rectification coordinate system.
   *
   *  \param [in] raw 2d position (col, row) in image raster coordinate system
   *  \param [out] rect 2d position (x, y) in rectification coordinate system
   *  \return Returns the corresponding model height if operation succeeds
   */
  std::optional<double> RawToRect(const Point2d &raw, Point2d &rect) const;

  // cached data for convenience
  Point3d mOrigin{0, 0, 0}; // longitude & latitude of origin of rectification coordinate system
  double mHeight{}; // height for doing epipolar rectficiation

  std::unique_ptr<InterpGridF> mGridI2G;    // height corrections in model CRS on a grid of image
  std::unique_ptr<InterpGridF> BuildGridI2G(int numCols, int numRows) const;
};

using GeoEpipolarModel = EpipolarModel<GeographicGroundDeg>;
using LocalEpipolarModel = EpipolarModel<LocalGroundMe>;
using MapEpipolarModel = EpipolarModel<MapGroundMe>;
} // namespace earthxpert
