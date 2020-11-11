/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/expackages.h"
#include <memory>

namespace earthxpert {
/**
 *
 */
class EXPLATFORM_EXPORTS ICommand {
public:
  virtual ~ICommand() = default;

  /**
   *  Entry point of a command
   */
  virtual int Execute() = 0;
};

using UPICommand = std::unique_ptr<ICommand>;
using SPICommand = std::shared_ptr<ICommand>;
}
