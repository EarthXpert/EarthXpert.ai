/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/exbba.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IBlockAdjust defines the behaviours of adjusting/calibrting
 *  imaging sensor systems.
 */
class EXBBA_EXPORTS IBlockAdjust {
public:
  virtual ~IBlockAdjust() = default;

  bool operator==(const IBlockAdjust&rhs) const = delete;
  bool operator!=(const IBlockAdjust&rhs) const = delete;
};

using UPIBlockAdjust = std::unique_ptr<IBlockAdjust>;
using SPIBlockAdjust = std::shared_ptr<IBlockAdjust>;
} // namespace earthxpert
