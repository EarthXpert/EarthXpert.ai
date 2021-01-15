/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/exbba.h"

#include <memory>

namespace earthxpert {
  /**
   *  The interface IBbaImage defines the behaviours of image bba info.
   */
  class EXBBA_EXPORTS IBbaImage {
  public:
    virtual ~IBbaImage() = default;

    bool operator==(const IBbaImage& rhs) const = delete;
    bool operator!=(const IBbaImage& rhs) const = delete;
  };

  using UPIBbaImage = std::unique_ptr<IBbaImage>;
  using SPIBbaImage = std::shared_ptr<IBbaImage>;
} // namespace earthxpert
