/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gts/delaunay2d.h"
#include <omp.h>

using namespace earthxpert;

void GtsDelaunay2d::GetGtsInfo(std::vector<DTVertex2f>& vertices,
  std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) {

}

void GtsDelaunay2d::GetPlyInfo(std::vector<DTVertex3f>& vertices,
  std::vector<DTTriangle>& tris) {

}
