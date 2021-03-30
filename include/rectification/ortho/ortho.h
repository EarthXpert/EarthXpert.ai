/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/ortho/exortho.h"

#include <string>
#include <vector>

namespace earthxpert {
class ISensorModel;
class IRasterResample;
class RasterReader;
class RasterWriter;

struct OrthoOptions {
  std::string sensorName;
  std::string imgFile;
  std::string mdlFile;

  std::vector<int> bands;
  std::string demFile;
  double ulPos[4]{};
  std::string ourSrs;
  double outGsd{};
  int outBkgd{ 0 };
  std::string outPath;
  int stride{ 8 };
  float rotAngle{};
  std::string resample;
  size_t memSize;  // [unit: bytes]
};

std::string EXORTHO_EXPORTS ToString(const OrthoOptions* orthoOpt);

void EXORTHO_EXPORTS GenerateFrameOrtho();

void EXORTHO_EXPORTS GenerateRationalOrtho(const OrthoOptions& orthoOpt);
} // namespace earthxpert
