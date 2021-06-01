/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/opencv/delaunay2d.h"
#include <omp.h>

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
 *  retrieve data structures for GTS
 *  \todo skip the fist 4 vertices
 */
void CvDelaunay2d::GetGtsInfo(std::vector<Point2f>& vertices,
  std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) {
  // each vertex is represented by two coordinates.
  assert(INT_MAX > vtx.size());
  const int numV = int(vtx.size());
  vertices.reserve(numV);

  for (int i = 0; i < numV; ++i) {
    vertices.emplace_back(vtx[i].pt + mOrigin);
  }

  mNumV = int(vertices.size());

  // each edge is represented by two Ids indexed into vertices array.
  assert(INT_MAX > qedges.size());
  const int numE = int(qedges.size());
  edges.reserve(std::max(numE - kVE, mNumV * 3));

  for (int i = kVE; i < numE; i++) {
    if (qedges[i].isfree()) continue;

    const int v0 = qedges[i].pt[0];
    const int v2 = qedges[i].pt[2];
    if (v0 > 0 && v2 > 0) {
      edges.emplace_back<DTEdge>({v0, v2});
    }
  }

  mNumE = int(edges.size());

  // each triangle is represented by three Ids indexed into edges array.
  assert(INT_MAX > qedges.size() * 4);
  const Rect2f rect(topLeft.x, topLeft.y,
    bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
  tris.reserve(std::max(mNumV * 2, mNumE * 2 / 3));

  const int numE4 = int(qedges.size() * 4);
  std::vector<bool> edgeMask(numE4, false);

  auto eqVtx = [](const DTEdge& e, const int v0, const int v1) {
    return (e.v0 == v0 && e.v1 == v1) || (e.v1 == v0 && e.v0 == v1);
  };

  Point2f v0, v1, v2;
  for (int i = kVE; i < numE4; i += 2) {
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
    int ei[3]{};

#pragma omp parallel sections num_threads(3)
    {
    #pragma omp section
      {
        for (size_t e = 0; e < mNumE; ++e) {
          if (eqVtx(edges[e], vi0, vi1)) {
            ei[0] = e;
            break;
          }
        }
      }

    #pragma omp section
      {
        for (size_t e = 0; e < mNumE; ++e) {
          if (eqVtx(edges[e], vi1, vi2)) {
            ei[1] = e;
            break;
          }
        }
      }

    #pragma omp section
      {
        for (size_t e = 0; e < mNumE; ++e) {
          if (eqVtx(edges[e], vi2, vi0)) {
            ei[2] = e;
            break;
          }
        }
      }
    }

    if (0 == ei[0] || 0 == ei[1] || 0 == ei[2]) continue;

    tris.emplace_back<DTTriangle>({ ei[0], ei[1], ei[2] });
  }

  mNumT = int(tris.size());
}

/**
 *  retrieve data structures for PLY
 */
void CvDelaunay2d::GetPlyInfo(std::vector<Point3f>& vertices,
  std::vector<DTTriangle>& tris) {
  // each vertex is represented by two coordinates.
  assert(INT_MAX > vtx.size());
  const int numV = int(vtx.size());
  vertices.reserve(numV - kVE);

  for (int i = kVE; i < numV; ++i) {
    vertices.emplace_back(vtx[i].pt + mOrigin);
  }

  mNumV = int(vertices.size());

  mNumE = std::max(int(qedges.size()) - kVE, mNumV * 3);

  // each triangle is represented by three Ids indexed into edges array.
  assert(INT_MAX > qedges.size() * 4);
  const Rect2f rect(topLeft.x, topLeft.y,
    bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
  tris.reserve(std::max(mNumV * 2, mNumE * 2 / 3));

  const int numE4 = int(qedges.size() * 4);
  std::vector<bool> edgeMask(numE4, false);

  Point2f v0, v1, v2;
  for (int i = kVE; i < numE4; i += 2) {
    if (edgeMask[i]) continue;

    const int em0 = i;
    int vi0 = edgeOrg(em0, &v0);
    if (!rect.contains(v0)) continue;

    const int em1 = getEdge(em0, NEXT_AROUND_LEFT);
    int vi1 = edgeOrg(em1, &v1);
    if (!rect.contains(v1)) continue;

    const int em2 = getEdge(em1, NEXT_AROUND_LEFT);
    int vi2 = edgeOrg(em2, &v2);
    if (!rect.contains(v2)) continue;

    edgeMask[em0] = true;
    edgeMask[em1] = true;
    edgeMask[em2] = true;

    vi0 -= kVE;
    vi1 -= kVE;
    vi2 -= kVE;
    if (0 > vi0 || 0 > vi1 || 0 > vi2) continue;

    tris.emplace_back<DTTriangle>({vi0, vi1, vi2});
  }

  mNumT = int(tris.size());
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
