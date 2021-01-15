/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/ceressolver.h"

namespace earthxpert {
/**
 *  The class RationalCeresSolver defines the behaviours of solving
 *  the optimization problem using Ceres functions.
 */
class EXBBA_EXPORTS RationalCeresSolver final : public CeresSolver {
public:
  virtual ~RationalCeresSolver() = default;
};

using UPRationalCeresSolver = std::unique_ptr<RationalCeresSolver>;
using SPRationalCeresSolver = std::shared_ptr<RationalCeresSolver>;
} // namespace earthxpert
