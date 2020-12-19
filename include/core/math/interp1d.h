/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/math/iinterp.h"

namespace earthxpert {
/**
 *  The class Interp1D implement the inter-/extra-polation behaviours.
 */
class EXCORE_EXPORTS Interp1D final : public IInterp {
public:
  ~Interp1D() = default;

  void operator()(double x, double* y, uint32_t len = UINT_MAX) const {
  }
};

using UPInterp1D = std::unique_ptr<Interp1D>;
using SPInterp1D = std::shared_ptr<Interp1D>;
}
