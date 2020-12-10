/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "frameworks/pipeline/itask.h"

#include <memory>

namespace earthxpert {
  /**
   *  The class TaskSchedular configures the task based pipeline,
   *  streamlines and runs tasks in parallel, possibly
   *  overlapping the data IO communication and the computation.
   */
  class EXFRAMEWORKS_EXPORTS TaskSchedular {
  public:
    ~TaskSchedular() = default;

  private:
    void Initialize();
  };

  using UPTaskSchedular = std::unique_ptr<TaskSchedular>;
  using SPTaskSchedular = std::shared_ptr<TaskSchedular>;
} // namespace earthxpert
