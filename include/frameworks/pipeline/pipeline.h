/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
 /* -----------------------------------------------------------------------------
  *  Copyright (c) 2019.  All rights reserved.
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "frameworks/exframeworks.h"
#include "3rdparty/taskflow/extaskflow.h"

#include <memory>

namespace earthxpert {
/**
 *  The class Pipeline implements the generic and pipelined
 *  task flows, enabling graph guided parallelism.
 */
class EXFRAMEWORKS_EXPORTS Pipeline {
public:
  Pipeline() = default;

private:
  void Initialize();
};

using UPPipeline = std::unique_ptr<Pipeline>;
using SPPipeline = std::shared_ptr<Pipeline>;
} // namespace earthxpert
