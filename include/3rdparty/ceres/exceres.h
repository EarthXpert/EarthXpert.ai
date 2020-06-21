/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include "ceres/ceres.h"
#include "ceres/tiny_solver.h"

namespace earthxpert::thirdparty {
// CostFunction categary
struct CeresTraits {
  // size of parameter block
  static constexpr unsigned int kDynamic = 0; // size set at run time
  static constexpr unsigned int kSized = 1;   // Size-fixed at compile time

  // differentation algorithm for CostFunction
  static constexpr unsigned int kAnalyticDiff = 0;    // analytic-differentiation
  static constexpr unsigned int kAutoDiff = 1;    // auto-differentiation framework
  static constexpr unsigned int kNumericDiff = 2; // numeric differentiation

  // bit fields
  unsigned int sized : 1;  // 0: kDynamic;  1: kSized
  unsigned int diff  : 1;  // 0: kAutoDiff; 1: kNumericDiff
};

// the interface IResidual defines the base of cost functor
struct IResidual {
  virtual ~IResidual() = default;
};
} // namespace earthxpert::thirdparty
