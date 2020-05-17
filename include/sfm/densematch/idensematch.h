/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/wxsfm.h"
#include "core/datasource/iraster.h"

namespace watxtract {

/**
 *  The interface IDenseMatch defines the behaviours of dense matching algorithms.
 */
class WXSFM_EXPORTS IDenseMatch {
public:
  enum MatchType {
    eDEPTH,
    eDISPARITY
  };

  virtual ~IDenseMatch() = default;

  virtual std::unique_ptr<IRaster> Match() = 0;

  virtual MatchType GetMatchType() const noexcept = 0;
};
}
