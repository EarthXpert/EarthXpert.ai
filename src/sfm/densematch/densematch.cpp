/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/densematch.h"
#include "core/exceptions/exexcept.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <execution>
#include <omp.h>

using earthxpert::DenseMatch;

/**
 *  \param [out] map the depth map where the element value represents the
 *  the depth of the corresonding pixel.
 *
 *  \return Retruns true if successful, false otherwise.
 */
bool DenseMatch::Process(IRaster* map) {
  try {
    LayoutTiles();
    MatchTiles();
  }
  catch (...) {
    throw;
  }

  return true;
}
