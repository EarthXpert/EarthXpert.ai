/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/opencv/delaunay2d.h"

using namespace earthxpert;
using namespace cv;

CvDelaunay2d::CvDelaunay2d(const cv::Rect& rect) 
  : Subdiv2D(rect) {
}

void CvDelaunay2d::EstimateStatistics() {
  mNumV = int(vtx.size()) - kVE;
  mNumE = std::max(int(qedges.size()) - kVE, mNumV * 3);
  mNumT = std::max(mNumV * 2, mNumE * 2 / 3);
  mNumP = mNumV;
}

/**
 *  each vertex is represented by two coordinates.
 *
 *  \return an array of vertices.
 */
std::vector<Point2f> CvDelaunay2d::GetVertices() const {
  assert(INT_MAX > vtx.size());
  const int total = int(vtx.size());

  std::vector<Point2f> vertices;
  vertices.reserve(total);

  // skip the fist 4 vertices
  for (int i = 0; i < total; ++i) {
    vertices.emplace_back(vtx[i].pt);
  }

  mNumV = int(vertices.size());
  return vertices;
}

/**
 *  each edge is represented by two Ids indexed into vertices array.
 *
 *  \return an array of edges.
 */
std::vector<DTEdge> CvDelaunay2d::GetEdges() const {
  assert(INT_MAX > qedges.size());
  const int total = int(qedges.size());

  std::vector<DTEdge> edges;
  edges.reserve(total);

  for (int i = kVE; i < total; i++) {
    if (qedges[i].isfree()) continue;

    const int v0 = qedges[i].pt[0];
    const int v2 = qedges[i].pt[2];
    if (v0 > 0 && v2 > 0) {
      edges.emplace_back<DTEdge>({v0, v2});
    }
  }

  mNumE = int(edges.size());
  return edges;
}

/**
 *  each triangle is represented by three Ids indexed into edges array.
 * 
 *  \return an array of triangles.
 */
std::vector<DTTriangle> 
CvDelaunay2d::GetTriangles(const std::vector<DTEdge>& edges) const {
  assert(INT_MAX > qedges.size()*4);
  const Rect2f rect(topLeft.x, topLeft.y,
    bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);

  const int total = int(qedges.size() * 4);
  std::vector<bool> edgeMask(total, false);

  std::vector<DTTriangle> tris;

  Point2f v0, v1, v2;
  for (int i = kVE; i < total; i += 2) {
    if (edgeMask[i]) continue;

    const int em0 = i;
    const int vi0 = edgeOrg(em0, &v0);
    if (!rect.contains(v0)) continue;

    const int em1 = getEdge(em0, NEXT_AROUND_LEFT);
    const int vi1 = edgeOrg(em1, &v1);
    if (!rect.contains(v1)) continue;

    const int em2 = getEdge(em1, NEXT_AROUND_LEFT);
    const int vi2 = edgeOrg(em2, &v2);
    if (!rect.contains(v2)) continue;

    edgeMask[em0] = true;
    edgeMask[em1] = true;
    edgeMask[em2] = true;

    if (kVE > vi0 || kVE > vi1 || kVE > vi2) continue;

    tris.emplace_back<DTTriangle>({vi0, vi1, vi2});
  }

  mNumT = int(tris.size());
  return tris;
}

/**
 *  each voronoi polygon is represented by a list of Ids indexed into vertices array.
 *
 *  \param [out] voronoi an array of polygons.
 *  \return an array of voronoi centers.
 */
std::vector<int> CvDelaunay2d::GetVoronoi(
  std::vector<std::vector<int>>& voronoi) const {
  std::vector<int> centers;

  mNumP = int(centers.size());
  return centers;
}
