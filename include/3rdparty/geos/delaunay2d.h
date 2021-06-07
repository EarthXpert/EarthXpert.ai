/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/geos/exgeos.h"
#include "3rdparty/idelaunay2d.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Class GeosDelaunay2d encapsulates GEOSGeometry.
 * 
 *  \note: implements QuadEdge but lower performance compared to OpenCV
 */
class EX3RDPARTY_EXPORTS GeosDelaunay2d : public IDelaunay2d {
public:
  ~GeosDelaunay2d() = default;

  void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) override;
  void GetPlyInfo(std::vector<DTVertex3f>& vertices, 
    std::vector<DTTriangle>& tris) override;

private:
};

using UPGeosDelaunay2d = std::unique_ptr<GeosDelaunay2d>;
using SPGeosDelaunay2d = std::shared_ptr<GeosDelaunay2d>;
}
