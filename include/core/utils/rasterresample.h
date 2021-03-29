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
  explicit RasterResample(const std::string& algo);
  ~RasterResample() = default;

  void SetBorderParams(cv::BorderTypes borderType, cv::Scalar borderValue) {
    mBorderType = borderType;
    mBorderValue = borderValue;
  }

  int GetWinDim() const noexcept override;

  void Resample(CvRaster& src, CvRaster* srcMask, 
    CvRaster& tgt, CvRaster* tgtMask, CvRaster& tfm) const override {
    assert(tgt.GetMarginSize() == tfm.GetMarginSize());
    Resample(src.GetCvMat(), nullptr, tgt.GetCvMat(), nullptr, tfm.GetCvMat());
  }

  inline void operator()(CvRaster& src, CvRaster* srcMask, 
    CvRaster& tgt, CvRaster* tgtMask, CvRaster& tfm) const {
    Resample(src, nullptr, tgt, nullptr, tfm);
  }

  void Resample(cv::Mat& src, cv::Mat* srcMask, 
    cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const override {
    assert(tgt.size() == tfm.size());
    if constexpr (eAB_CV == algoBackend) {
      ResampleCv(src, nullptr, tgt, nullptr, tfm);
    }
    else if constexpr (eAB_EXAI == algoBackend) {
      ResampleExai(src, nullptr, tgt, nullptr, tfm);
    }
    else {
      throw std::invalid_argument(_("RasterResample: backend algorithm not supported"));
    }
  }

  inline void operator()(cv::Mat& src, cv::Mat* srcMask, 
    cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const {
    Resample(src, nullptr, tgt, nullptr, tfm);
  }

private:
  cv::InterpolationFlags mInterpAlgo{ cv::INTER_CUBIC }; // interpolation algorithm
  cv::BorderTypes mBorderType{ cv::BORDER_CONSTANT };
  cv::Scalar mBorderValue{ cv::Scalar::all(0) };

  void ResampleCv(cv::Mat& src, cv::Mat* srcMask, 
    cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const;
  void ResampleExai(cv::Mat& src, cv::Mat* srcMask, 
    cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const;
};

using RasterResampleCv = RasterResample<IRasterResample::eAB_CV>;
using RasterResampleExai = RasterResample<IRasterResample::eAB_EXAI>;
}
