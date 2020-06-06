/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/idensematch.h"

namespace earthxpert {

/**
 *  The class DenseMatch implements the template methods for Semi-global matching algorithm.
 */
class EXSFM_EXPORTS DenseMatch : public IDenseMatch {
public:
  virtual ~DenseMatch() = default;

protected:
  // smooth condition depending on cost type and loss type
  double mP1{};  // a low penalty for continuity
  double mP2{};  // a larger penalty for dis-continuity

  int mStride{ 1 }; // interval between two rigorously matched pixels

  void SetPenalty(const double P1, const double P2) {
    mP1 = P1;
    mP2 = P2;
  }
};
} // namespace earthxpert
