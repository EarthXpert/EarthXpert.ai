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

  void SetPenalty(const double P1, const double P2) {
    mP1 = P1;
    mP2 = P2;
  }

  /**
   *  \param [out] map the depth map where the element value represents the
   *  the depth of the corresonding pixel.
   *
   *  \return Retruns true if successful, false otherwise.
   */
  bool Match(IRaster* map) override;

protected:
  // smooth condition depending on cost type and loss type
  double mP1{};  // a low penalty for continuity
  double mP2{};  // a larger penalty for dis-continuity

  MatchDim mMatchDim{ e1D };
  int mStride{ 1 }; // interval between two rigorously matched pixels

  virtual int LayoutTiles() = 0;
  virtual int MatchTiles() = 0;
};
} // namespace earthxpert
