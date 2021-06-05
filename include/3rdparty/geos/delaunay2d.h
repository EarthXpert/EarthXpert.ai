/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/geos/exgeos.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GeosDelaunay2d encapsulates GEOSGeometry.
 */
class EX3RDPARTY_EXPORTS GeosDelaunay2d {
public:
  ~GeosDelaunay2d() = default;
};

using UPGeosDelaunay2d = std::unique_ptr<GeosDelaunay2d>;
using SPGeosDelaunay2d = std::shared_ptr<GeosDelaunay2d>;
}
