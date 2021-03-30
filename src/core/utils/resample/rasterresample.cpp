/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/rasterresample.h"
#include "core/exceptions/exexcept.h"

#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <type_traits>

using namespace earthxpert;

namespace {
  // interpolation algorithms
  const std::map<std::string, cv::InterpolationFlags> sInterpAlgoMap = {
    {"nearest", cv::INTER_NEAREST},
    {"linear", cv::INTER_LINEAR},
    {"cubic", cv::INTER_CUBIC},
    {"lanczos", cv::INTER_LANCZOS4} };

  // dimension of interpolation window
  const std::map<cv::InterpolationFlags, int> sInterpWinMap = {
    {cv::INTER_NEAREST, 1}, {cv::INTER_LINEAR, 2},
    {cv::INTER_CUBIC, 4}, {cv::INTER_LANCZOS4, 8} };
}

/**
 *  constructor
 *
 *  \brief construct a resampler for the given interpolation algorithm.
 */
template <int algoBackend>
RasterResample<algoBackend>::RasterResample(const std::string& algo) {
  auto it = sInterpAlgoMap.find(algo);
  if (it == sInterpAlgoMap.end()) 
    throw std::invalid_argument(_("RasterResample: algo"));
  mInterpAlgo = it->second;
}

/**
 *  \return Returns dimension of resampling window
 */
template <int algoBackend>
int RasterResample<algoBackend>::GetWinDim() const noexcept {
  return sInterpWinMap.at(mInterpAlgo);
}

/**
 *  \brief Call cv::remap() to wrap the source raster into the target raster
 *  using the specified 2D position transformation raster.
 *
 *  \param [out] tgt target to populate resampled values. 
 *               tgt mask: the tfm value (-1, -1).
 *  \param [in] tfm Resampling transformation of target raster in 
 *              source raster. if source raster has margins, the 2D positions
 *              should have been tanslated accordingly to be relative to the
 *              upper-left corner of the source buffer. 
 *
 *  \todo src mask.
 */
template <int algoBackend>
void RasterResample<algoBackend>::ResampleCv(cv::Mat& src, cv::Mat* srcMask,
  cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const {
  // erose tgtMask if srcMask presents nodata pixels and 
  // reset corresponding erosen pixels in tfm as (-1,-1)
  if (srcMask) {
    bool hasBkgd{}; // AnyOf
    for (auto it = srcMask->begin<uchar>(); it != srcMask->end<uchar>(); ++it) {
      if (0 == *it) {
        hasBkgd = true;
        break;
      }
    }
    if (hasBkgd) {

    }
  }
  cv::remap(src, tgt, tfm, cv::Mat(), mInterpAlgo, mBorderType, mBorderValue);
}

template <int algoBackend>
void RasterResample<algoBackend>::ResampleExai(cv::Mat& src, cv::Mat* srcMask,
  cv::Mat& tgt, cv::Mat* tgtMask, cv::Mat& tfm) const {
}

template class earthxpert::RasterResample<IRasterResample::eAB_CV>;
template class earthxpert::RasterResample<IRasterResample::eAB_EXAI>;
