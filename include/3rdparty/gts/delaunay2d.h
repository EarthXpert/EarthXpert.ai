/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gts/exgts.h"
#include "3rdparty/idelaunay2d.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GtsDelaunay2d encapsulates GTS Surface.
 */
class EX3RDPARTY_EXPORTS GtsDelaunay2d final : public IDelaunay2d {
public:
  ~GtsDelaunay2d() = default;

  void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) override;
  void GetPlyInfo(std::vector<DTVertex3f>& vertices, 
    std::vector<DTTriangle>& tris) override;

private:
  GtsSurface* mTin{};
};

using UPGtsDelaunay2d = std::unique_ptr<GtsDelaunay2d>;
using SPGtsDelaunay2d = std::shared_ptr<GtsDelaunay2d>;
}
