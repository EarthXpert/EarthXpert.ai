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
  // match dimension
  enum MatchDim {
    e1D,    // 1D along x direction
    e2D     // 2D in both x and y directions
  };

  enum MatchType {
    eDEPTH,     // match depth in ground space
    eDISPARITY  // match disparity in image space
  };

  enum PathType {
    ePT8,     // 8 pathes
    ePY16     // 16 pathes
  };

  virtual ~IDenseMatch() = default;

  /**
   *  \param [out] depth the depth map where the element value represents the
   *  the depth of the corresonding pixel.
   *
   *  \return Retruns true if successful, false otherwise.
   */
  virtual bool Match(IRaster*) = 0;

  virtual MatchType GetMatchType() const noexcept = 0;
};
}
