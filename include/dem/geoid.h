/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "dem/igeoid.h"

namespace earthxpert {
class EXDEM_EXPORTS Geoid final : public IGeoid {
public:
  Geoid();
  ~Geoid() = default;
};
}
