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
class EXIO_EXPORTS IDemWriter {
public:
  virtual ~IDemWriter() = default;
};

using UPIDemWriter = std::unique_ptr<IDemWriter>;
using SPIDemWriter = std::shared_ptr<IDemWriter>;
}
