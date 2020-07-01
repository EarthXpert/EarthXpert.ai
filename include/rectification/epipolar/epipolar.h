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

void EXEPIPOLAR_EXPORTS GenerateFrameEpipolar();

void EXEPIPOLAR_EXPORTS GenerateRationalEpipolar(
  const std::string& leftImgFile, const std::string& leftMdlFile,
  const std::string& rightImgFile, const std::string& rightMdlFile,
  const std::vector<int>& bands, int sample, const std::string& outPath,
  const std::string& resample, size_t memSize);
} // namespace earthxpert::epipolar
