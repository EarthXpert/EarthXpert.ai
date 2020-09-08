/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/utils/irasterresample.h"
#include "core/exceptions/exexcept.h"

#include <string>

namespace earthxpert {
/**
 *  The class RasterResample implements behavior given a resampling kernel
 *  using OpenCV remap as backend.
 *
 *  \tparam algoBackend Specify the backend implementation
 */
template <int algoBackend = IRasterResample::eAB_CV>
class EXCORE_EXPORTS RasterResample final : public IRasterResample {
public:
  RasterResample(const std::string& algo);
  ~RasterResample() = default;

  void SetBorderParams(cv::BorderTypes borderType, cv::Scalar borderValue) {
    mBorderType = borderType;
    mBorderValue = borderValue;
  }

  int GetWinDim() const noexcept override;

  void Resample(CvRaster& srcRaster, CvRaster& tgtRaster, CvRaster& tfmRaster) const override {
    if constexpr(eAB_CV == algoBackend) {
      ResampleCV(srcRaster, tgtRaster, tfmRaster);
    }
    else {
      throw std::invalid_argument(_("RasterResample: backend algorithm not supported"));
    }
  }

  void Resample(cv::Mat& srcMat, cv::Mat& tgtMat, cv::Mat& tfmMat) const override {
    if constexpr(eAB_CV == algoBackend) {
      ResampleCV(srcMat, tgtMat, tfmMat);
    }
    else {
      throw std::invalid_argument(_("RasterResample: backend algorithm not supported"));
    }
  }

private:
  cv::InterpolationFlags mInterpAlgo{ cv::INTER_CUBIC }; // interpolation algorithm
  cv::BorderTypes mBorderType{ cv::BORDER_CONSTANT };
  cv::Scalar mBorderValue{ cv::Scalar::all(0) };

  void ResampleCV(CvRaster& srcRaster, CvRaster& tgtRaster, CvRaster& tfmRaster) const;
  void ResampleCV(cv::Mat& srcMat, cv::Mat& tgtMat, cv::Mat& tfmMat) const;
};

using RasterResampleCV = RasterResample<IRasterResample::eAB_CV>;
}
