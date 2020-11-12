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

// interpolation algorithms
static const std::map<std::string, cv::InterpolationFlags> sInterpAlgoMap = {
  {"nearest", cv::INTER_NEAREST},
  {"linear", cv::INTER_LINEAR},
  {"cubic", cv::INTER_CUBIC},
  {"lanczos", cv::INTER_LANCZOS4} };

// dimension of interpolation window
static const std::map<cv::InterpolationFlags, int> sInterpWinMap = {
  {cv::INTER_NEAREST, 1}, {cv::INTER_LINEAR, 2}, 
  {cv::INTER_CUBIC, 4}, {cv::INTER_LANCZOS4, 8} };

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
 *  \param [in] tfm Resampling transformation of target raster in 
 *              source raster. if source raster has margins, the 2D positions
 *              should have been tanslated accordingly to be relative to the
 *              upper-left corner of the source buffer.
 */
template <int algoBackend>
void RasterResample<algoBackend>::ResampleCV(CvRaster& src,
  CvRaster& tgt, CvRaster& tfm) const {
  assert(tgt.GetSize() == tfm.GetSize());
  assert(tgt.GetMarginSize() == tfm.GetMarginSize());
  cv::remap(src.GetCvMat(), tgt.GetCvMat(), tfm.GetCvMat(), 
    cv::Mat(), mInterpAlgo, mBorderType, mBorderValue);
}

/**
 *  \param [in] tfmMat Resampling transformation of target raster in source raster.
 */
template <int algoBackend>
void RasterResample<algoBackend>::ResampleCV(cv::Mat& src, 
  cv::Mat& tgt, cv::Mat& tfm) const {
  assert(tgt.size() == tfm.size());
  cv::remap(src, tgt, tfm, cv::Mat(), mInterpAlgo, mBorderType, mBorderValue);
}

template class earthxpert::RasterResample<IRasterResample::eAB_CV>;
