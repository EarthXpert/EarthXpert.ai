/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/rasterresample.h"
#include "3rd_party/opencv/exopencv.h"

#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <stdexcept>
#include <type_traits>

using namespace earthxpert;

/**
 *  constructor
 */
RasterResample::RasterResample(std::string algo)
  : mAlgo(std::move(algo)) {
}

/**
 *  \brief
 */
void RasterResample::Resample() {
}
