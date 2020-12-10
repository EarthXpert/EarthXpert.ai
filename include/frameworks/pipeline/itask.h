/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "frameworks/exframeworks.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface ITask defines behaviours of IO communication and 
 *  algorithmic computation.
 */
class EXFRAMEWORKS_EXPORTS ITask {
public:
  virtual ~ITask() = default;

  virtual void ReadTile() = 0;
  virtual void ProcessTile() = 0;
  virtual void WriteTile() = 0;
};

using UPITask = std::unique_ptr<ITask>;
using SPITask = std::shared_ptr<ITask>;
} // namespace earthxpert
