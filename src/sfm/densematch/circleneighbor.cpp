/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/circleneighbor.h"

using earthxpert::CircleNeighbor;

CircleNeighbor::CircleNeighbor(const int radius)
  : mRadius(radius) {
  GenerateIndices();
}

void CircleNeighbor::GenerateIndices() {
  if (mRadius < 1) mRadius = 1;

}
