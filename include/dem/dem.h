/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"

namespace earthxpert {
/**
 *  \brief the mixin Dem implements template methods of Digital Elevation Model.
 */
class EXDEM_EXPORTS Dem : public IDem {
public:
  virtual ~Dem() = default;
};
}
