/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

namespace earthxpert {

/**
 *  The interface INeighborhood defines the behaviour of neighborhood selection.
 */
class EXSFM_EXPORTS INeighborhood {
public:
  enum NeighborType {
    eCIRCLE,    // circular
    eRECT     // rectangular
  };

  virtual ~INeighborhood() = default;

  virtual NeighborType GetNeighborType() const noexcept = 0;
};
}
