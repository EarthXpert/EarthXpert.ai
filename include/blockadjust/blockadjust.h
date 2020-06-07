/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/iblockadjust.h"

namespace earthxpert {
/**
 *  The base class BlockAdjust implements the template methods of adjustment.
 */
class EXBBA_EXPORTS BlockAdjust : public IBlockAdjust {
public:
  virtual ~BlockAdjust() = default;
};
} // namespace earthxpert
