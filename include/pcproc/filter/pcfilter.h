/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "pcproc/filter/ipcfilter.h"

namespace earthxpert {
/**
 *  The base class PCFilter defines the behaviours of template methods.
 */
class EXPCPROC_EXPORTS PCFilter : public IPCFilter {
public:
  virtual ~PCFilter() = default;
};
}
