/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/ineighborhood.h"
#include <vector>

namespace watxtract {

/**
 *  The interface RectNeighbor defines the rectangular neighborhood selection.
 */
class WXSFM_EXPORTS RectNeighbor final : public INeighborhood {
public:
  RectNeighbor(int width, int height);
  ~RectNeighbor() = default;

  auto& GetIndices() const noexcept {
    return mIndices;
  }

  NeighborType GetNeighborType() const noexcept {
    return eRECT;
  }

private:
  int mWidth{};
  int mHeight{};
  std::vector<int> mIndices;

  void GenerateIndices();
};
}
