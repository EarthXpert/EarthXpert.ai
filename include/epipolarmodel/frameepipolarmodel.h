/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"

#include <memory>
#include <utility>

#define matrixSize 9

namespace earthxpert {
class ISensorModel;

/**
 *  The class FrameEpipolarModel defines the xxx algorithm
 *  for generating epipolar images from image products with frame models.
 */
class EXEPIPOLARMODEL_EXPORTS FrameEpipolarModel final : public IEpipolarModel {
public:
  /**
   *  Constructor.
   */
  FrameEpipolarModel(std::shared_ptr<ISensorModel> leftModel,
                     std::shared_ptr<ISensorModel> rightModel);
  ~FrameEpipolarModel() {}

  /**
   *  Epipolar-to-image transform.
   *
   *  \brief Given a 2d poistion on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in/out] img 2d position (x, y) in image raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  bool EpipolarToImage(const Point2d &epi, Point2d &img,
                       StereoPos pos) const override;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster coordinate
   * system \param [in] img List of 2d positions (x, y) in image raster
   * coordinate system \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  size_t EpipolarToImage(const std::vector<Point2d> &epi,
                         std::vector<Point2d> &img,
                         StereoPos pos) const override;

  /**
   *  Image-to-epipolar transform.
   *
   *  \brief Given a 2d poistion on image, output the corresponding position on
   * epipolar.
   *
   *  \param [in] img 2d position (x, y) in image raster coordinate system
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  bool ImageToEpipolar(const Point2d &img, Point2d &epi,
                       StereoPos pos) const override;

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] img List of 2d positions (x, y) in image raster coordinate
   * system \param [in] epi List of 2d positions (x, y) in epipolar raster
   * coordinate system \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  size_t ImageToEpipolar(const std::vector<Point2d> &img,
                         std::vector<Point2d> &epi,
                         StereoPos pos) const override;

  /**
   *  \return Returns algorithm of epipolar model
   */
  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return eCOPLANAR;
  }

private:
  std::shared_ptr<ISensorModel>
      mLeftModel; // left image composing of the stereo model
  std::shared_ptr<ISensorModel> mRightModel; // right image

  // cached data for convenience
};
} // namespace earthxpert
