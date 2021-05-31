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
 *  Class CvDelaunay2d extends Subdiv2D.
 */
class EX3RDPARTY_EXPORTS CvDelaunay2d : public cv::Subdiv2D {
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

  CvDelaunay2d(const cv::Rect& rect);
  ~CvDelaunay2d() = default;

  void EstimateStatistics();
  std::vector<Point2f> GetVertices() const;
  std::vector<DTEdge> GetEdges() const;
  std::vector<DTTriangle> GetTriangles(const std::vector<DTEdge>& edges) const;
  std::vector<int> GetVoronoi(std::vector<std::vector<int>>& voronoi) const;

private:
  static constexpr int kVE = 4;

  //Point2f mOrigin{};

  // estimated statistics
  mutable int mNumV{};  // vertices
  mutable int mNumE{};  // edges
  mutable int mNumT{};  // triangles
  mutable int mNumP{};  // voronoi polygons
};

using DTEdge = CvDelaunay2d::DTEdge;
using DTTriangle = CvDelaunay2d::DTTriangle;

using UPCvDelaunay2d = std::unique_ptr<CvDelaunay2d>;
using SPCvDelaunay2d = std::shared_ptr<CvDelaunay2d>;
}
