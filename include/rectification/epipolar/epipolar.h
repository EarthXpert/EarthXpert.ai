/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/wxepipolar.h"

#include <memory>
#include <string>
#include <vector>

namespace earthxpert {
class IEpipolarModel;
class IRasterResample;
class ISensorModel;
class RasterReader;
class RasterWriter;

namespace epipolar {
void WXEPIPOLAR_EXPORTS GenerateEpipolar(
  RasterReader* leftReader, const ISensorModel* leftModel,
  RasterReader* rightReader, const ISensorModel* rightModel,
  const IEpipolarModel* epipolarModel,
  RasterWriter* leftEpiWriter, RasterWriter* rightEpiWriter,
  const IRasterResample* rasResample, size_t memSize);

void WXEPIPOLAR_EXPORTS GenerateFrameEpipolar();

void WXEPIPOLAR_EXPORTS GenerateRationalEpipolar(
  const std::string& leftImgFile, const std::string& leftMdlFile,
  const std::string& rightImgFile, const std::string& rightMdlFile,
  const std::vector<int>& bands, int sample, const std::string& outPath,
  const std::string& resample, size_t memSize);
}
} // namespace watxtract::epipolar
