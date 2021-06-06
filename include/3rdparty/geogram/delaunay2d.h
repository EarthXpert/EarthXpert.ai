/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/geogram/exgeogram.h"
#include "3rdparty/types.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GeogramDelaunay2d encapsulates .
 */
class EX3RDPARTY_EXPORTS GeogramDelaunay2d : public GEO::Delaunay2d {
public:
  //GeogramDelaunay2d() : GEO::Delaunay2d() {}
  ~GeogramDelaunay2d() = default;

  void Insert(std::vector<double>&& vertices) {
    mVertices = std::move(vertices);
    set_vertices((unsigned)mVertices.size()/2, mVertices.data());
  }

  void GetPlyInfo(std::vector<DTVertex3f>& vertices, std::vector<DTTriangle>& tris);

private:
  DTVertex2f mOrigin;
  std::vector<double> mVertices;  // sequence of xy coordinates
};

using UPGeogramDelaunay2d = std::unique_ptr<GeogramDelaunay2d>;
using SPGeogramDelaunay2d = std::shared_ptr<GeogramDelaunay2d>;
}
