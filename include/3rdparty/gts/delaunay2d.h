/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gts/exgts.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GtsDelaunay2d encapsulates GTS Surface.
 */
class EX3RDPARTY_EXPORTS GtsDelaunay2d {
public:
  // edge described by start and end vertices Ids indexed into vertices array
  struct DTVertex { 
    float x{};
    float y{};
  };

  // triangle described by three edge Ids indexed into edges array
  // or three vertex Ids indexed into vertices array
  struct DTTriangle {  
    int t0{}; // 1st edge/vertex
    int t1{}; // 2nd edge/vertex
    int t2{}; // 3rd edge/vertex
  };

  ~GtsDelaunay2d() = default;

  void SetOrigin(const DTVertex& origin) { mOrigin = origin; }

  DTVertex GetOrigin() const noexcept { return mOrigin; }

  void GetPlyInfo(std::vector<DTVertex>& vertices, std::vector<DTTriangle>& tris);

private:
  DTVertex mOrigin;
  GtsSurface* mSurface{};
};

using UPGtsDelaunay2d = std::unique_ptr<GtsDelaunay2d>;
using SPGtsDelaunay2d = std::shared_ptr<GtsDelaunay2d>;
}
