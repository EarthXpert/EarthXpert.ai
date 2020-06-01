/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"
#include "core/datasource/iraster.h"
#include <utility>

namespace earthxpert {

/**
 *  The interface IDenseMatch defines the behaviours of dense matching algorithms.
 */
class EXSFM_EXPORTS IDenseMatch {
public:
  // match algorithm
  enum MatchAlgo {
    eSGM,   // semi-global matching in image space
    eMVS    // semi-global matching in ground space
  };

  // match dimension
  enum MatchDim {
    e1D,    // 1D along x direction
    e2D     // 2D in both x and y directions
  };

  enum MatchType {
    eDEPTH,     // match depth
    eDISPARITY  // match disparity
  };

  virtual ~IDenseMatch() = default;

  /**
   *  \return Retruns a map representing the disparity or depth.
   */
  virtual std::unique_ptr<IRaster> Match() = 0;

  virtual MatchType GetMatchType() const noexcept = 0;
};
}
