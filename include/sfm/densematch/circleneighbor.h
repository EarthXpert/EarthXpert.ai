/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/ineighborhood.h"
#include <vector>

namespace earthxpert {

/**
 *  The interface CircleNeighbor defines the behaviour of circular neighborhood selection.
 */
class EXSFM_EXPORTS CircleNeighbor final : public INeighborhood {
public:
  CircleNeighbor(int radius);
  ~CircleNeighbor() = default;

  auto& GetIndices() const noexcept {
    return mIndices;
  }

  NeighborType GetNeighborType() const noexcept {
    return eCIRCLE;
  }

private:
  int mRadius{};

  // cache
  std::vector<int> mIndices;

  void GenerateIndices();
};
} // namespace earthxpert
