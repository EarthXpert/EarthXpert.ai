/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
/**
 *  The interface IRasterResample defines behavior given a resampling kernel.
 */
class EXCORE_EXPORTS IRasterResample {
public:
  virtual ~IRasterResample() {}

  virtual void Resample() = 0;
};
}
