/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarsensormodel.h"
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
class EXEPIPOLARMODEL_EXPORTS EpipolarModel final : public EpipolarSensorModel {
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
  EpipolarModel(std::shared_ptr<ISensorModel> sensorModel, StereoPos pos,
    std::unique_ptr<EpipolarRect> &&epiRect, std::shared_ptr<GroundPolicy> groundPolicy);
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
   *  \return Returns algorithm of epipolar model
   */
  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return ePLANERECT;
  }

  const EpipolarRect *GetEpiRect(StereoPos pos) const noexcept {
    return mEpiRect.get();
  }

  Point3d GetOrigin() const noexcept { return mOrigin; }

private:
  static constexpr int kMaxIters{ 5 };
  static constexpr double kHgtTol{ 0.0001 }; // Height tolerance [0.1mm]?
  static constexpr int kGridStride{ 256 };

  std::unique_ptr<EpipolarRect> mEpiRect;     // epipolar rectification
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
