/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/utils/irasterresample.h"
#include <string>

namespace watxtract {
/**
 *  The class RasterResample implements behavior given a resampling kernel.
 */
class WXCORE_EXPORTS RasterResample : public IRasterResample {
public:
  RasterResample(std::string algo);

  void Resample() override;

private:
  std::string mAlgo;
};
}
