/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/blockadjust.h"
#include "sensormodel/rationalmodel.h"

namespace earthxpert {
/**
 *  The class RationalBBA defines the behaviours of adjusting
 *  rational function model based universal imaging systems.
 */
class EXBBA_EXPORTS RationalBBA final : public BlockAdjust {
public:
  ~RationalBBA() = default;
};
} // namespace earthxpert
