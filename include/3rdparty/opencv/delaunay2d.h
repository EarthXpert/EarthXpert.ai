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
  // or three vertex Ids indexed into vertices array
  struct DTTriangle {   // Point3i
    int t0{}; // 1st edge/vertex
    int t1{}; // 2nd edge/vertex
    int t2{}; // 3rd edge/vertex
  };

  CvDelaunay2d(const cv::Rect& rect);
  ~CvDelaunay2d() = default;

  void SetOrigin(const Point2f& origin) { mOrigin = origin;  }

  void EstimateStatistics();
  std::vector<int> GetVoronoi(std::vector<std::vector<int>>& voronoi) const;

  Point2f GetOrigin() const noexcept { return mOrigin; }

  void GetGtsInfo(std::vector<Point2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris);
  void GetPlyInfo(std::vector<Point2f>& vertices, std::vector<DTTriangle>& tris);

private:
  static constexpr int kVE = 4;

  Point2f mOrigin{};

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
