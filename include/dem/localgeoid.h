/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "dem/igeoid.h"

namespace earthxpert {
class EXDEM_EXPORTS LocalGeoid final : public IGeoid {
public:
  LocalGeoid();
  ~LocalGeoid() = default;
};
}
