/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

namespace earthxpert {
template <class GeometryModel, class CostFunc, class Neighborhood>
DisparityMatch<GeometryModel, CostFunc, Neighborhood>::DisparityMatch() {
}

template <class GeometryModel, class CostFunc, class Neighborhood>
int DisparityMatch<GeometryModel, CostFunc, Neighborhood>::LayoutTiles() {
  return 0;
}

template <class GeometryModel, class CostFunc, class Neighborhood>
int DisparityMatch<GeometryModel, CostFunc, Neighborhood>::MatchTiles() {
  return 0;
}
}

#include "disparitymatch_cv.hh"
#include "disparitymatch_vxl.hh"
