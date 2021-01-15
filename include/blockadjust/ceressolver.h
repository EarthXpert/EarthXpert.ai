/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/isolver.h"
#include "3rdparty/ceres/exceres.h"

namespace earthxpert {
/**
 *  The class CeresSolver defines the behaviours of solving
 *  the optimization problem using Ceres functions.
 */
class EXBBA_EXPORTS CeresSolver : public ISolver {
public:
  virtual ~CeresSolver() = default;
};
} // namespace earthxpert
