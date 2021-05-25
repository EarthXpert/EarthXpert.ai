/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include <opencv2/imgproc.hpp>

namespace earthxpert {
/**
 *  Class DelaunayTri2d extends Subdiv2D.
 */
class EX3RDPARTY_EXPORTS DelaunayTri2d : public cv::Subdiv2D {
public:
  // edge described by start and end vertices Ids indexed into vertices array
  struct DTEdge {
    int v0{}; // start vertex
    int v1{}; // end vertex
  };

  // triangle described by three edge Ids indexed into edges array
  struct DTTriangle {
    int e0{}; // 1st edge
    int e1{}; // 2bd esge
    int e2{}; // 3rd edge
  };

  DelaunayTri2d(const cv::Rect& rect) : Subdiv2D(rect) {}
  ~DelaunayTri2d() = default;

  const std::vector<Vertex>& GetVertices() const { return vtx; }
  const std::vector<DTEdge> GetEdges() const;
  const std::vector<DTTriangle> GetTriangles() const;

private:
};

using DTEdge = DelaunayTri2d::DTEdge;
using DTTriangle = DelaunayTri2d::DTTriangle;
}
