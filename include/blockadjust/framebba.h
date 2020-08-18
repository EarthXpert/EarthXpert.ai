/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/blockadjust.h"

namespace earthxpert {
/**
 *  The class FrameBBA defines the behaviours of adjusting/calibrting
 *  pin-hole model based frame imaging systems.
 */
class EXBBA_EXPORTS FrameBBA final : public BlockAdjust {
public:
  ~FrameBBA() = default;
};

using UPFrameBBA = std::unique_ptr<FrameBBA>;
using SPFrameBBA = std::shared_ptr<FrameBBA>;
} // namespace earthxpert
