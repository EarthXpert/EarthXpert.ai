/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/exio.h"

#include <memory>

namespace exarthxpert {
/**
 *
 */
class EXIO_EXPORTS IDemReader {
public :
  virtual ~IDemReader() = default;
};

using UPIDemReader = std::unique_ptr<IDemReader>;
using SPIDemReader = std::shared_ptr<IDemReader>;
}
