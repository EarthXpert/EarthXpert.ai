/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "pcproc/expcproc.h"
#include "core/datasource/pointcloud.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IPCFilter defines the behaviours of point cloud processing algorithms.
 */
class EXPCPROC_EXPORTS IPCFilter {
public:
  virtual ~IPCFilter() = default;

  virtual bool Process(Pointcloud* pc) = 0;
};

using UPIPCFilter = std::unique_ptr<IPCFilter>;
using SPIPCFilter = std::shared_ptr<IPCFilter>;
}
