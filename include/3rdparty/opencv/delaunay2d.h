/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include "3rdparty/idelaunay2d.h"
#include <opencv2/imgproc.hpp>
#include <memory>

namespace earthxpert {
/**
 *  Class CvDelaunay2d extends Subdiv2D.
 */
class EX3RDPARTY_EXPORTS CvDelaunay2d final
  : public cv::Subdiv2D, public IDelaunay2d {
public:
  CvDelaunay2d(const cv::Rect& rect);
  ~CvDelaunay2d() = default;

  int Insert(const float x, const float y) { return insert(Point2f{x, y}); }
  int Delete(float x, float y);

  int Locate(const Point2f& p, DTTriangle& tri);

  void EstimateStatistics();
  std::vector<int> GetVoronoi(std::vector<std::vector<int>>& voronoi) const;

  void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) override;
  void GetPlyInfo(std::vector<DTVertex3f>& vertices, 
    std::vector<DTTriangle>& tris) override;

  size_t GetNumBytes() const override {
    const size_t memSize = sizeof(Vertex) * vtx.size() + 
      sizeof(QuadEdge) * qedges.size();
    return memSize;
  }

private:
  static constexpr int kVE = 4;

  int Locate(const Point2f& p, int& ei, int& vi);
};

using UPCvDelaunay2d = std::unique_ptr<CvDelaunay2d>;
using SPCvDelaunay2d = std::shared_ptr<CvDelaunay2d>;
}
