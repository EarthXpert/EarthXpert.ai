/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/blockadjust.h"

namespace earthxpert {
/**
 *  The class AdsBBA defines the behaviours of adjusting/calibrting
 *  aerial pushbroom scanner based multi-line imaging systems.
 */
class EXBBA_EXPORTS AdsBBA final : public BlockAdjust {
public:
  ~AdsBBA() = default;
};
} // namespace earthxpert
