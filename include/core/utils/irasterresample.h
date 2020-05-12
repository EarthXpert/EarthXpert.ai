/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"

namespace watxtract {
/**
 *  The interface IRasterResample defines behavior given a resampling kernel.
 */
class WXCORE_EXPORTS IRasterResample {
public:
  virtual ~IRasterResample() {}

  virtual void Resample() = 0;
};
}
