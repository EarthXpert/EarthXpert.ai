/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarsensormodel.h"
#include "sensormodel/framemodel.h"

namespace earthxpert {
/**
 *  The class FrameEpipolarModel defines the rigorous algorithm
 *  for generating epipolar images from image products with frame models.
 *
 *  \param ImageModelPolicy defines the relationship between
 * the image raster coordinate system and the focal plane coordinate system
 */
template <class ImageModelPolicy>
class EXEPIPOLARMODEL_EXPORTS FrameEpipolarModel final : public EpipolarSensorModel {
public:
  /**
   *  \param [in] pos Component position of stereo model
   *  \param [in] sensorModel sensor model associated with the image
   *  \param [in] epiBound epipolar boundary of valid region
   */
  FrameEpipolarModel(StereoPos pos,
    std::shared_ptr<FrameModel<ImageModelPolicy>> sensorModel, 
    const Polygon2d& epiBound);
  ~FrameEpipolarModel() = default;

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
  bool EpiToRaw(const Point2d &epi, Point2d &raw) const override;

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
  bool RawToEpi(const Point2d &raw, Point2d &epi) const override;

  /**
   *  \return Returns algorithm of epipolar model
   */
  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return eCOPLANAR;
  }

  /**
   *  \return Returns the internal sensor model
   */
  const FrameModel<ImageModelPolicy>* 
    GetSensorModel() const noexcept override { return mFrameModel; }

  /**
   *  \return Returns raster columns & rows of epipolar image [unit: pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return { mNumCols, mNumRows };
  }

private:
  int mNumCols{ 0 };       // Number of cols of epipolar image
  int mNumRows{ 0 };       // Number of rows of epipolar image

  void Initialize();

  // cached data for convenience
  const FrameModel<ImageModelPolicy>* mFrameModel{};
};

using DigitalFrameEpipolarModel = FrameEpipolarModel<DigitalImageModel>;
using FilmFrameEpipolarModel = FrameEpipolarModel<FilmImageModel>;
} // namespace earthxpert
