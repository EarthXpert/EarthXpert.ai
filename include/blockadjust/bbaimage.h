/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/ibbaimage.h"

namespace earthxpert {
  /**
   *  The base class BbaImage implements the template methods.
   */
  class EXBBA_EXPORTS BbaImage : public IBbaImage {
  public:
    virtual ~BbaImage() = default;
  };
} // namespace earthxpert
