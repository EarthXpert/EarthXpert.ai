/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface ISparseMatch defines the behaviours of sparse matching algorithms.
 */
class EXSFM_EXPORTS ISparseMatch {
public:
  virtual ~ISparseMatch() = default;
};

using UPISparseMatch = std::unique_ptr<ISparseMatch>;
using SPISparseMatch = std::shared_ptr<ISparseMatch>;
}
