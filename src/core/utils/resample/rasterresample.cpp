/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/rasterresample.h"
#include "3rdparty/gettext/exgettext.h"

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
  if (it == sInterpAlgoMap.end()) {
    throw std::invalid_argument(_("RasterResample: algo"));
  }
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
 *  using the specified 2D position transformation raster:
 *
 *  \param [in] posRaster Resampling positions of target raster in source raster.
 */
template <int algoBackend>
void RasterResample<algoBackend>::ResampleCV(CvRaster& srcRaster,
  CvRaster& tgtRaster, CvRaster& posRaster) const {
  assert(tgtRaster.GetSize() == posRaster.GetSize());
  assert(tgtRaster.GetMarginSize() == posRaster.GetMarginSize());
  cv::remap(srcRaster.GetCvMat(), tgtRaster.GetCvMat(), posRaster.GetCvMat(), 
    cv::Mat(), mInterpAlgo, mBorderType, mBorderValue);
}

/**
 *  \param [in] posMat Resampling positions of target raster in source raster.
 */
template <int algoBackend>
void RasterResample<algoBackend>::ResampleCV(cv::Mat& srcMat, cv::Mat& tgtMat, cv::Mat& posMat) const {
  assert(tgtMat.size() == posMat.size());
  cv::remap(srcMat, tgtMat, posMat, cv::Mat(), mInterpAlgo, mBorderType, mBorderValue);
}

template class earthxpert::RasterResample<IRasterResample::eAB_CV>;
