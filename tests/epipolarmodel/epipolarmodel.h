/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/opencv/wxopencv.h"
#include "core/math/interpgrid.h"
#include "epipolarmodel/epipolarrect.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "epipolarmodel/groundpolicy.h"
#include "sensormodel/isensormodel.h"

#include <optional>

namespace watxtract {

/**
 *  The class EpipolarModel defines a sensor-independent algorithm
 *  for generating epipolar images from image products with sensor models.
 *
 *  \param GroundPolicy defines the behaviour of and relationshp between
 * the model coordinate system and the rectification coordinate system
 */
template <class GroundPolicy>
class WXEPIPOLARMODEL_EXPORTS EpipolarModel final : public IEpipolarModel {
public:
  using PolicyType = GroundPolicy;

  /**
   *  Constructor.
   *
   *  \param [in] leftModel sensor model associated with the left image
   *  \param [in] leftRect epipolar rectification defining the transform between
   * the epipolar model coordinate system and epipolar raster coordinate system
   *              for the left image
   *  \param [in] rightModel sensor model associated with the right image
   *  \param [in] rightRect epipolar rectification defining the transform
   * between the epipolar model coordinate system and epipolar raster coordinate
   * system for the right image
   */
  EpipolarModel(std::shared_ptr<ISensorModel> leftModel,
                std::unique_ptr<EpipolarRect> &&leftRect,
                std::shared_ptr<ISensorModel> rightModel,
                std::unique_ptr<EpipolarRect> &&rightRect,
                std::unique_ptr<GroundPolicy> &&groundPolicy);
  ~EpipolarModel() = default;

  /**
   *  Epipolar-to-image transform.
   *
   *  \brief Given a 2d poistion on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
   *  \param [in] img 2d position (col, row) in image raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  bool EpipolarToImage(const Point2d &epi, Point2d &img,
                       StereoPos pos) const override;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (col, row) in epipolar raster coordinate
   * system \param [in] img List of 2d positions (col, row) in image raster
   * coordinate system \param [in] pos Component position of stereo model
   *  \return Returns number of successful operations
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
   *  \param [in] img 2d position (col, row) in image raster coordinate system
   *  \param [in] epi 2d position (col, row) in epipolar raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  bool ImageToEpipolar(const Point2d &img, Point2d &epi,
                       StereoPos pos) const override;

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] img List of 2d positions (col, row) in image raster coordinate
   * system 
   *  \param [in] epi List of 2d positions (col, row) in epipolar raster
   * coordinate system 
   *  \param [in] pos Component position of stereo model
   *  \return Returns number of successful operations
   */
  size_t ImageToEpipolar(const std::vector<Point2d> &img,
                         std::vector<Point2d> &epi,
                         StereoPos pos) const override;

  /**
   *  \return Returns algorithm of epipolar model
   */
  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return ePLANERECT;
  }

  const EpipolarRect *GetEpiRect(StereoPos pos) const noexcept {
    return (pos == StereoPos::eLEFT) ? mLeftRect.get() : mRightRect.get();
  }

  Point3d GetOrigin() const noexcept { return mOrigin; }

private:
  std::shared_ptr<ISensorModel>
      mLeftModel; // left image composing of the stereo model
  std::unique_ptr<EpipolarRect> mLeftRect;   // left epipolar rectification
  std::shared_ptr<ISensorModel> mRightModel; // right image
  std::unique_ptr<EpipolarRect> mRightRect;  // right epipolar rectification
  std::unique_ptr<GroundPolicy> mGroundPolicy;

  int mMaxIters{ 5 };
  double mHgtTol{ 0.0001 }; // Height tolerance [0.1mm]?

  /**
   *  Image-to-rect transform.
   *
   *  \brief Given a 2d poistion on image, output the 2d position in
   * rectification coordinate system.
   *
   *  \param [in] img 2d position (col, row) in image raster coordinate system
   *  \param [out] rect 2d position (x, y) in rectification coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns the corresponding model height if operation succeeds
   */
  std::optional<double> ImageToRect(const Point2d &img, Point2d &rect, StereoPos pos) const;

  // cached data for convenience
  Point3d mOrigin{0, 0, 0}; // longitude & latitude of origin of rectification coordinate system
  double mHeight{}; // height for doing epipolar rectficiation

  int mGridStride{256};
  std::unique_ptr<InterpGridF> mLeftGridI2G;    // height corrections in model CRS on a grid of left image
  std::unique_ptr<InterpGridF> mRightGridI2G;   // height corrections in model CRS on a grid of right image
  std::unique_ptr<InterpGridF> BuildGridI2G(StereoPos pos, int numCols, int numRows) const;
};

using GeoEpipolarModel = EpipolarModel<GeographicGround>;
using LocalEpipolarModel = EpipolarModel<LocalGround>;
using MapEpipolarModel = EpipolarModel<MapGround>;
} // namespace watxtract
