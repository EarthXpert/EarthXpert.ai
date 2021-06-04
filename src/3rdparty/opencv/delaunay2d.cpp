/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/opencv/delaunay2d.h"
#include <omp.h>
#include <numeric>

using namespace earthxpert;
using namespace cv;

CvDelaunay2d::CvDelaunay2d(const cv::Rect& rect) 
  : Subdiv2D(rect) {
}

/**
 *  Locate the triangle containing the given point.
 * 
 *  \param [in] p the point to locate
 *  \param [out] tri three verices indices if inside, the first two
 *               vertices if on edge, the fist vertex if coincide.
 * 
 *  \return Return 
 */
int CvDelaunay2d::Locate(const Point2f& p, DTTriangle& tri) {
  int e0{}, v0{};
  const int loc = Locate(p, e0, v0);

  if (PTLOC_VERTEX == loc) {
    tri.t0 = v0;
  }
  else if (PTLOC_ON_EDGE == loc) {
    tri.t0 = edgeOrg(e0);
    tri.t1 = edgeDst(e0);
  }
  else {  // PTLOC_INSIDE
    tri.t0 = v0;
    const int e1 = getEdge(e0, NEXT_AROUND_LEFT);
    tri.t1 = edgeOrg(e1);
    const int e2 = getEdge(e1, NEXT_AROUND_LEFT);
    tri.t2 = edgeOrg(e2);
  }
  return loc;
}

void CvDelaunay2d::EstimateStatistics() {
  mNumV = int(vtx.size()) - kVE;
  mNumE = std::max(int(qedges.size()) - kVE, mNumV * 3);
  mNumT = std::max(mNumV * 2, mNumE * 2 / 3);
  mNumP = mNumV;
}

/**
 *  retrieve data structures for GTS
 */
void CvDelaunay2d::GetGtsInfo(std::vector<Point2f>& vertices,
  std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) {
  // each vertex is represented by two coordinates.
  assert(INT_MAX > vtx.size());
  const int numV = int(vtx.size());
  vertices.reserve(numV);

  for (int i = kVE; i < numV; ++i) {
    vertices.emplace_back(vtx[i].pt + mOrigin);
  }
  mNumV = int(vertices.size());

  // each edge is represented by two Ids indexed into vertices array.
  assert(INT_MAX > qedges.size());
  const int numE = int(qedges.size());
  edges.reserve(std::max(numE - kVE, mNumV * 3));

  std::vector<int> eis;   // edge indices logic ruler
  eis.reserve(std::max(numE - kVE, mNumV * 3));

  for (int i = kVE; i < numE; i++) {
    if (qedges[i].isfree()) continue;

    int v0 = qedges[i].pt[0];
    int v2 = qedges[i].pt[2];
    if (0 >= v0 || 0 >= v2) continue;

    v0 -= kVE;
    v2 -= kVE;
    if (0 <= v0 && 0 <= v2) {
      edges.emplace_back<DTEdge>({ v0, v2 });
      eis.push_back(1);
    }
    else {
      eis.push_back(0);
    }
  }
  mNumE = int(edges.size());

  auto it = std::find(eis.begin(), eis.end(), 1);
  *it = 0;
  std::partial_sum(eis.begin(), eis.end(), eis.begin());

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

    const int ei0 = em0 / kVE - kVE;
    const int ei1 = em1 / kVE - kVE;
    const int ei2 = em2 / kVE - kVE;
    if (0 > ei0 || 0 > ei1 || 0 > ei2) continue;

    tris.emplace_back<DTTriangle>({ eis[ei0], eis[ei1], eis[ei2] });
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
    int vi0 = edgeOrg(em0, &v0) - kVE;
    if (!rect.contains(v0)) continue; // ? 0 > vi0

    const int em1 = getEdge(em0, NEXT_AROUND_LEFT);
    int vi1 = edgeOrg(em1, &v1) - kVE;
    if (!rect.contains(v1)) continue;

    const int em2 = getEdge(em1, NEXT_AROUND_LEFT);
    int vi2 = edgeOrg(em2, &v2) - kVE;
    if (!rect.contains(v2)) continue;

    edgeMask[em0] = true;
    edgeMask[em1] = true;
    edgeMask[em2] = true;

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

int CvDelaunay2d::Locate(const Point2f& p, int& ei, int& vi) {
  if (qedges.size() < 4 || p.x < topLeft.x || p.y < topLeft.y ||
    p.x >= bottomRight.x || p.y >= bottomRight.y) {
    return PTLOC_ERROR;
  }

  ei = recentEdge;

  int right_of_curr = isRightOf(p, ei);
  if (0 < right_of_curr) {
    ei = symEdge(ei);
    right_of_curr = -right_of_curr;
  }

  const int numE4 = int(qedges.size()) * 4;
  int loc = PTLOC_ERROR;

  for (int i = 0; i < numE4; i++) {
    int onext_edge = nextEdge(ei);
    int dprev_edge = getEdge(ei, PREV_AROUND_DST);

    int right_of_onext = isRightOf(p, onext_edge);
    int right_of_dprev = isRightOf(p, dprev_edge);

    if (0 < right_of_dprev) {
      if (0 < right_of_onext || (0 == right_of_onext && 0 == right_of_curr)) {
        loc = PTLOC_INSIDE;
        break;
      }
      else {
        right_of_curr = right_of_onext;
        ei = onext_edge;
      }
    }
    else {
      if (0 < right_of_onext) {
        if (0 == right_of_dprev && 0 == right_of_curr) {
          loc = PTLOC_INSIDE;
          break;
        }
        else {
          right_of_curr = right_of_dprev;
          ei = dprev_edge;
        }
      }
      else if (0 == right_of_curr &&
        isRightOf(vtx[edgeDst(onext_edge)].pt, ei) >= 0) {
        ei = symEdge(ei);
      }
      else {
        right_of_curr = right_of_onext;
        ei = onext_edge;
      }
    }
  }

  recentEdge = ei;
  vi = 0;

  if (PTLOC_INSIDE == loc) {
    Point2f po, pd;
    const int vio = edgeOrg(ei, &po);
    const int vid = edgeDst(ei, &pd);

    const double d1 = fabs(p.x - po.x) + fabs(p.y - po.y);
    const double d2 = fabs(p.x - pd.x) + fabs(p.y - pd.y);
    const double d3 = fabs(po.x - pd.x) + fabs(po.y - pd.y);

    auto triArea = [](Point2f a, Point2f b, Point2f c) {
      return ((double)b.x - a.x) * ((double)c.y - a.y) - 
        ((double)b.y - a.y) * ((double)c.x - a.x);
    };

    if (FLT_EPSILON > d1) {
      loc = PTLOC_VERTEX;
      ei = 0;
      vi = vio;
    }
    else if (FLT_EPSILON > d2) {
      loc = PTLOC_VERTEX;
      ei = 0;
      vi = vid;
    }
    else if ((d1 < d3 || d2 < d3) && fabs(triArea(p, po, pd)) < FLT_EPSILON) {
      loc = PTLOC_ON_EDGE;
    }
  }

  return loc;
}
