/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/exbba.h"

#include <memory>

namespace earthxpert {
  /**
   *  The interface ISolver defines the behaviours of solving algebraic systems.
   */
  class EXBBA_EXPORTS ISolver {
  public:
    virtual ~ISolver() = default;
    ISolver(const ISolver&) = delete;
    ISolver(ISolver&&) = delete;

    ISolver& operator=(const ISolver&) = delete;
    ISolver& operator=(ISolver&&) = delete;

    bool operator==(const ISolver& rhs) const = delete;
    bool operator!=(const ISolver& rhs) const = delete;
  };

  using UPISolver = std::unique_ptr<ISolver>;
  using SPISolver = std::shared_ptr<ISolver>;
} // namespace earthxpert
