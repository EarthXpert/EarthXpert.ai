/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/geogram/delaunay2d.h"
#include <omp.h>

using namespace earthxpert;

void GeogramDelaunay2d::GetPlyInfo(std::vector<DTVertex3f>& vertices, 
  std::vector<DTTriangle>& tris) {
  const int numV = int(mVertices.size()) / 2;
  vertices.reserve(numV);

  for (int i = 0; i < numV; ++i) {
    const int i2 = i * 2;
    vertices.emplace_back<DTVertex3f>({float(mVertices[i2]), float(mVertices[i2+1]), 0.f});
  }

  tris.reserve(numV * 2);
	
  const int* tri = cell_to_v();
	for (int i = 0; i < nb_cells(); ++i) {
		tris.emplace_back<DTTriangle>({ tri[0], tri[1], tri[2] });
		tri += 3;
	}
}
