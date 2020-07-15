/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/utils/irasterresample.h"
#include <string>

namespace earthxpert {
/**
 *  The class RasterResample implements behavior given a resampling kernel
 *  using OpenCV remap as backend.
 */
class EXCORE_EXPORTS RasterResample final : public IRasterResample {
public:
  RasterResample(const std::string& algo);
  ~RasterResample() = default;

  void SetBorderParams(cv::BorderTypes borderType, cv::Scalar borderValue) {
    mBorderType = borderType;
    mBorderValue = borderValue;
  }

  int GetWinDim() const noexcept override;

  void Resample(CvRaster& srcRaster, CvRaster& tgtRaster, CvRaster& posRaster) const override;
  void Resample(cv::Mat& srcRaster, cv::Mat& tgtRaster, cv::Mat& posRaster) const override;

private:
  cv::InterpolationFlags mInterpAlgo{ cv::INTER_CUBIC }; // interpolation algorithm
  cv::BorderTypes mBorderType{ cv::BORDER_CONSTANT };
  cv::Scalar mBorderValue{ cv::Scalar::all(0) };
};
}
