/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IInterp defines the inter-/extra-polation behaviours.
 */
class EXCORE_EXPORTS IInterp {
public:
  virtual ~IInterp() = default;
};

using UPIInterp = std::unique_ptr<IInterp>;
using SPIInterp = std::shared_ptr<IInterp>;
}
