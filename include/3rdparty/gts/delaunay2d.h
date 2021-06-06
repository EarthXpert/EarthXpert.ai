/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gts/exgts.h"
#include "3rdparty/types.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GtsDelaunay2d encapsulates GTS Surface.
 */
class EX3RDPARTY_EXPORTS GtsDelaunay2d {
public:
  ~GtsDelaunay2d() = default;

  void SetOrigin(const DTVertex2f& origin) { mOrigin = origin; }

  DTVertex2f GetOrigin() const noexcept { return mOrigin; }

  void GetPlyInfo(std::vector<DTVertex3f>& vertices, std::vector<DTTriangle>& tris);

private:
  DTVertex2f mOrigin;
  GtsSurface* mSurface{};
};

using UPGtsDelaunay2d = std::unique_ptr<GtsDelaunay2d>;
using SPGtsDelaunay2d = std::shared_ptr<GtsDelaunay2d>;
}
