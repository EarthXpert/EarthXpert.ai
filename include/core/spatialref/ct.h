/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/ict.h"

namespace earthxpert {
/**
 *  The class Ct implements template methods of coordinate transformation
 */
class EXCORE_EXPORTS Ct : public ICt {
public:
  virtual ~Ct() = default;

  inline bool operator()(double& x, double& y, double& z) {
    return Transform(x, y, z);
  }
};
}
