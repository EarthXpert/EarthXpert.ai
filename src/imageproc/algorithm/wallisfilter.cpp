/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "imageproc/algorithm/imgenhance.h"
#include "core/datasource/cvraster.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

namespace earthxpert {
bool WallisFilterCv(CvRaster* raster, const WallisParams* wp) {
  auto& ras = raster->GetCvMat();
  //ras.forEach([]() {});
  return true;
}
}
