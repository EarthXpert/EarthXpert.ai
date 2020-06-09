/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include "ceres/ceres.h"

namespace earthxpert::thirdparty {
// the interface IResidual defines the base of cost functor
struct IResidual {
  ~IResidual() = default;
};
} // namespace earthxpert::thirdparty
