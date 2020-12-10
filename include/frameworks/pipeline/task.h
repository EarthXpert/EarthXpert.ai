/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "frameworks/pipeline/itask.h"

namespace earthxpert {
/**
 *  The class Task implements the template methods.
 */
class EXFRAMEWORKS_EXPORTS Task : public ITask {
public:
  virtual ~Task() = default;

private:
  void Initialize();
};

using UPTask = std::unique_ptr<Task>;
using SPTask = std::shared_ptr<Task>;
} // namespace earthxpert
