/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/geos/delaunay2d.h"

#include <omp.h>

using namespace earthxpert;

/**
 *  \param [in] cs an array of points
 */
void GeosDelaunay2d::Insert(const gsg::CoordinateSequence& cs) {
  mTin.setTolerance(FLT_EPSILON);
  mTin.setSites(cs);
  mTin.getSubdivision();
}

void GeosDelaunay2d::GetGtsInfo(std::vector<DTVertex2f>& vertices,
  std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) {

}

void GeosDelaunay2d::GetPlyInfo(std::vector<DTVertex3f>& vertices,
  std::vector<DTTriangle>& tris) {
  auto gFac = gsg::GeometryFactory::getDefaultInstance();
  auto gTris = mTin.getTriangles(*gFac);
}
