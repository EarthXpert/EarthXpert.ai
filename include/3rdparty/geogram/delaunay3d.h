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
 *  Class GeogramDelaunay3d encapsulates .
 */
class EX3RDPARTY_EXPORTS GeogramDelaunay3d : public GEO::Delaunay3d {
public:
  ~GeogramDelaunay3d() = default;

private:
  DTVertex3f mOrigin;
  std::vector<double> mVertices;  // sequence of xyz coordinates
};

using UPGeogramDelaunay3d = std::unique_ptr<GeogramDelaunay3d>;
using SPGeogramDelaunay3d = std::shared_ptr<GeogramDelaunay3d>;
}
