/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include "3rdparty/types.h"
#include <opencv2/imgproc.hpp>
#include <memory>

namespace earthxpert {
/**
 *  Class CvDelaunay2d extends Subdiv2D.
 */
class EX3RDPARTY_EXPORTS CvDelaunay2d : public cv::Subdiv2D {
public:
  CvDelaunay2d(const cv::Rect& rect);
  ~CvDelaunay2d() = default;

  void SetOrigin(const DTVertex2f& origin) { mOrigin = origin;  }
  int Insert(const float x, const float y) { return insert(Point2f{x, y}); }

  int Locate(const Point2f& p, DTTriangle& tri);

  void EstimateStatistics();
  std::vector<int> GetVoronoi(std::vector<std::vector<int>>& voronoi) const;

  DTVertex2f GetOrigin() const noexcept { return mOrigin; }

  void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris);
  void GetPlyInfo(std::vector<DTVertex3f>& vertices, std::vector<DTTriangle>& tris);

private:
  static constexpr int kVE = 4;

  DTVertex2f mOrigin{};

  int Locate(const Point2f& p, int& ei, int& vi);

  // estimated statistics
  mutable int mNumV{};  // vertices
  mutable int mNumE{};  // edges
  mutable int mNumT{};  // triangles
  mutable int mNumP{};  // voronoi polygons
};

using UPCvDelaunay2d = std::unique_ptr<CvDelaunay2d>;
using SPCvDelaunay2d = std::shared_ptr<CvDelaunay2d>;
}
