/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/epipolarsensormodel.h"

namespace earthxpert {
/**
 *  The class FrameEpipolarModel defines the rigorous algorithm
 *  for generating epipolar images from image products with frame models.
 */
class EXEPIPOLARMODEL_EXPORTS FrameEpipolarModel final : public EpipolarSensorModel {
public:
  /**
   *  \param [in] sensorModel sensor model associated with the image
   *  \param [in] pos Component position of stereo model
   */
  FrameEpipolarModel(std::shared_ptr<ISensorModel> sensorModel, StereoPos pos);
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

private:
  void Initialize();

  // cached data for convenience
};
} // namespace earthxpert
