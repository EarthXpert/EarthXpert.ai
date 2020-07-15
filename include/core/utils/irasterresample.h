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
  virtual ~IRasterResample() = default;

  virtual int GetWinDim() const noexcept = 0;

  virtual void Resample(CvRaster& srcRaster, CvRaster& tgtRaster, CvRaster& posRaster) const = 0;
  virtual void Resample(cv::Mat& srcRaster, cv::Mat& tgtRaster, cv::Mat& posRaster) const = 0;
};
}
