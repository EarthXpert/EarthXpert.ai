/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/datasource/cvraster.h"

#include <map>

namespace earthxpert {
/**
 *  The interface IRasterResample defines behavior given a resampling kernel.
 */
class EXCORE_EXPORTS IRasterResample {
public:
  enum AlgoBackend {
    eAB_CV,   // call cv::remap
    eAB_EXAI
  };

  virtual ~IRasterResample() = default;

  virtual int GetWinDim() const noexcept = 0;

  virtual void Resample(CvRaster& srcRaster, CvRaster* srcMask, 
    CvRaster& tgtRaster, CvRaster* tgtMask, CvRaster& posRaster) const = 0;
  virtual void Resample(cv::Mat& srcMat, cv::Mat* srcMask, 
    cv::Mat& tgtMat, cv::Mat* tgtMask, cv::Mat& posMat) const = 0;
};

using UPIRasterResample = std::unique_ptr<IRasterResample>;
using SPIRasterResample = std::shared_ptr<IRasterResample>;
}
