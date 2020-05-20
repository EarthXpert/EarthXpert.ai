/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/idensematch.h"

namespace watxtract {

/**
 *  The class DenseMatch implements the template methods for Semi-global matching algorithm.
 */
class WXSFM_EXPORTS DenseMatch : public IDenseMatch {
public:
  virtual ~DenseMatch() = default;

protected:
  void SetPenalty(const int P1, const int P2) {
    mP1 = P1;
    mP2 = P2;
  }

private:
  int mP1{};  // smooth condition: a low penalty for continuity
  int mP2{};  //                   a larger penalty for dis-continuity 
};
}
