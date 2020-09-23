/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/eximageproc.h"
#include "core/datasource/iraster.h"

#include <memory>

namespace earthxpert {
class CvRaster;

struct CloudParams {
};

struct WaterBodyParams {
};

struct WallisParams {
  float ms;   // mean of source
  float ds;   // std
  float mt;   // mean of target
  float dt;   // std
};

struct DehazeParams {
};

struct SuperResParams {
};

bool EXIMAGEPROC_EXPORTS CloudDetectCv(CvRaster* raster, const CloudParams* cp);
bool EXIMAGEPROC_EXPORTS WaterBodyDetectCv(CvRaster* raster, const WaterBodyParams* wp);

bool EXIMAGEPROC_EXPORTS WallisFilterCv(CvRaster* raster, const WallisParams* wp);
bool EXIMAGEPROC_EXPORTS DehazeCv(CvRaster* raster, const DehazeParams* wp);

bool EXIMAGEPROC_EXPORTS SuperResolutionCv(CvRaster* raster, const SuperResParams* wp);
}
