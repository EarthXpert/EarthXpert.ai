/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"

#include <string>
#include <vector>

namespace earthxpert {
class IEpipolarModel;
class IRasterResample;
class RasterReader;
class RasterWriter;

namespace epipolar {
void EXEPIPOLAR_EXPORTS GenerateEpipolar(
  RasterReader* leftReader, const IEpipolarModel* leftEpiModel,
  RasterReader* rightReader, const IEpipolarModel* rightEpiModel,
  RasterWriter* leftEpiWriter, RasterWriter* rightEpiWriter,
  const IRasterResample* rasResample, size_t memSize);

void EXEPIPOLAR_EXPORTS GenerateFrameEpipolar();

void EXEPIPOLAR_EXPORTS GenerateRationalEpipolar(
  const std::string& leftImgFile, const std::string& leftMdlFile,
  const std::string& rightImgFile, const std::string& rightMdlFile,
  const std::vector<int>& bands, int sample, const std::string& outPath,
  const std::string& resample, size_t memSize);
}
} // namespace earthxpert::epipolar
