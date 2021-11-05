/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/sparsematch/isparsematch.h"

namespace earthxpert {
/**
 *  The class SparseMatch implements the template methods.
 */
class EXSFM_EXPORTS SparseMatch : public ISparseMatch {
public:
  virtual ~SparseMatch() = default;
};

using UPSparseMatch = std::unique_ptr<SparseMatch>;
using SPSparseMatch = std::shared_ptr<SparseMatch>;
}
