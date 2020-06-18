/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
/**
 * The interface IJob defines the behaviour for running a load.
 */
class EXCORE_EXPORTS IJob {
public:
  virtual ~IJob() = default;

  /**
   *  Entry point of a job
   */
  virtual int Run() = 0;
};
}
