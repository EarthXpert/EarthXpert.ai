/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sfm/densematch/rectneighbor.h"

using watxtract::RectNeighbor;

RectNeighbor::RectNeighbor(const int width, const int height)
  : mWidth(width), mHeight(height) {
  GenerateIndices();
}

void RectNeighbor::GenerateIndices() {
}
