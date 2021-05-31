/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gts/exgts.h"
#include <memory>

namespace earthxpert {
/**
 *  Class GtsDelaunay2d encapsulates GTS Surface.
 */
class EX3RDPARTY_EXPORTS GtsDelaunay2d {
public:
  ~GtsDelaunay2d() = default;

private:
  GtsSurface* mSurface{};
};

using UPGtsDelaunay2d = std::unique_ptr<GtsDelaunay2d>;
using SPGtsDelaunay2d = std::shared_ptr<GtsDelaunay2d>;
}
