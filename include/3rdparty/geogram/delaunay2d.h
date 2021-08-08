/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/geogram/exgeogram.h"
#include "3rdparty/idelaunay2d.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GeogramDelaunay2d encapsulates .
 */
class EX3RDPARTY_EXPORTS GeogramDelaunay2d final
  : public GEO::Delaunay2d, public IDelaunay2d {
public:
  //GeogramDelaunay2d() : GEO::Delaunay2d() {}
  ~GeogramDelaunay2d() = default;

  void Insert(std::vector<double>&& vertices) {
    mVertices = std::move(vertices);
    set_vertices((unsigned)mVertices.size()/2, mVertices.data());
  }

  void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) override;
  void GetPlyInfo(std::vector<DTVertex3f>& vertices, 
    std::vector<DTTriangle>& tris) override;

  size_t GetNumBytes() const override {
    return 0;
  }

private:
  std::vector<double> mVertices;  // sequence of xy coordinates
};

using UPGeogramDelaunay2d = std::unique_ptr<GeogramDelaunay2d>;
using SPGeogramDelaunay2d = std::shared_ptr<GeogramDelaunay2d>;
}
