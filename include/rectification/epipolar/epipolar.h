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

struct EpipolarOptions {
  std::string sensorName;
  std::string leftImgFile;
  std::string leftMdlFile;
  std::string nadImgFile;
  std::string nadMdlFile;
  std::string rightImgFile;
  std::string rightMdlFile;

  std::vector<int> bands;
  int sample{ 1 };
  int stride{ 8 };
  bool grey{ false };     // convert color image into grey
  bool oneFile{ false };  // store all epipolar images in one file
  int outBkgd{ 0 };
  std::string outOpt;
  std::string outPath;
  std::string resample;
  size_t memSize;  // [unit: bytes]
};

std::string EXEPIPOLAR_EXPORTS ToString(const EpipolarOptions* epiOpt);

void EXEPIPOLAR_EXPORTS GenerateFrameEpipolar();

void EXEPIPOLAR_EXPORTS GenerateRationalEpipolar(const EpipolarOptions& epiOpt);
} // namespace earthxpert::epipolar
