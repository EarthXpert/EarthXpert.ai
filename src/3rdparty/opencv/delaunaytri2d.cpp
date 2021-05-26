/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/opencv/delaunaytri2d.h"

using namespace earthxpert;
using namespace cv;

DelaunayTri2d::DelaunayTri2d(const cv::Rect& rect) 
  : Subdiv2D(rect) {
}

/**
 *  each vertex is represented by two coordinates.
 *
 *  \return an array of vertices.
 */
std::vector<Point2f> DelaunayTri2d::GetVertices() const {
  return {};
}

/**
 *  each edge is represented by two Ids indexed into vertices array.
 *
 *  \return an array of edges.
 */
std::vector<DTEdge> DelaunayTri2d::GetEdges() const {
  std::vector<DTEdge> edges;

  for (size_t i = 4; i < qedges.size(); i++) {
    if (qedges[i].isfree())
      continue;
    if (qedges[i].pt[0] > 0 && qedges[i].pt[2] > 0) {
      edges.emplace_back<DTEdge>({qedges[i].pt[0], qedges[i].pt[2]});
    }
  }

  return edges;
}

/**
 *  each triangle is represented by three Ids indexed into edges array.
 * 
 *  \return an array of triangles.
 */
std::vector<DTTriangle> DelaunayTri2d::GetTriangles() const {
  const Rect2f rect(topLeft.x, topLeft.y,
    bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
  std::vector<DTTriangle> tris;

  const int total = int(qedges.size() * 4);
  std::vector<bool> edgeMask(total, false);

  Point2f v0, v1, v2;
  for (int i = 4; i < total; i += 2) {
    if (edgeMask[i]) continue;

    const int e0 = i;
    edgeOrg(e0, &v0);
    if (!rect.contains(v0)) 
      continue;

    const int e1 = getEdge(e0, NEXT_AROUND_LEFT);
    edgeOrg(e1, &v1);
    if (!rect.contains(v1))
      continue;

    const int e2 = getEdge(e1, NEXT_AROUND_LEFT);
    edgeOrg(e2, &v2);
    if (!rect.contains(v2))
      continue;

    edgeMask[e0] = true;
    edgeMask[e1] = true;
    edgeMask[e2] = true;

    tris.emplace_back<DTTriangle>({e0 , e1 , e2});
  }

  return tris;
}

/**
 *  each voronoi polygon is represented by a list of Ids indexed into vertices array.
 *
 *  \param [out] voronoi an array of polygons.
 *  \return an array of voronoi centers.
 */
std::vector<int> DelaunayTri2d::GetVoronoi(
  std::vector<std::vector<int>>& voronoi) const {
  std::vector<int> centers;
  return centers;
}
