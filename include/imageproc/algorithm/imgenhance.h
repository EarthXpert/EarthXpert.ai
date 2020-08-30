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

struct WallisParams {
  float ms;   // mean of source
  float ds;   // std
  float mt;   // mean of target
  float dt;   // std
};

bool EXIMAGEPROC_EXPORTS WallisFilterCv(CvRaster* raster, const WallisParams* wp);
}
