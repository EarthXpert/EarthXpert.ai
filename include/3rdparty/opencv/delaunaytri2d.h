/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include <opencv2/imgproc.hpp>
#include <memory>

namespace earthxpert {
/**
 *  Class DelaunayTri2d extends Subdiv2D.
 */
class EX3RDPARTY_EXPORTS DelaunayTri2d : public cv::Subdiv2D {
public:
  // edge described by start and end vertices Ids indexed into vertices array
  struct DTEdge {   // Point2i
    int v0{}; // from vertex
    int v1{}; // to vertex
  };

  // triangle described by three edge Ids indexed into edges array
  struct DTTriangle {   // Point3i
    int e0{}; // 1st edge
    int e1{}; // 2nd edge
    int e2{}; // 3rd edge
  };

  DelaunayTri2d(const cv::Rect& rect);
  ~DelaunayTri2d() = default;

  std::vector<Point2f> GetVertices() const;
  std::vector<DTEdge> GetEdges() const;
  std::vector<DTTriangle> GetTriangles() const;
  std::vector<int> GetVoronoi(std::vector<std::vector<int>>& voronoi) const;

private:
};

using DTEdge = DelaunayTri2d::DTEdge;
using DTTriangle = DelaunayTri2d::DTTriangle;

using UPDelaunayTri2d = std::unique_ptr<DelaunayTri2d>;
using SPDelaunayTri2d = std::shared_ptr<DelaunayTri2d>;
}
