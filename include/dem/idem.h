/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/exdem.h"
#include <memory>

namespace earthxpert {
/**
 *
 */
class EXDEM_EXPORTS IDem {
public:
  virtual ~IDem() = default;
};

using UPIDem = std::unique_ptr<IDem>;
using SPIDem = std::shared_ptr<IDem>;
}
