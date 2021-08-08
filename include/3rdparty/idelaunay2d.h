/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include "3rdparty/types.h"
#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  Interface IDelaunay2d.
 */
class EX3RDPARTY_EXPORTS IDelaunay2d {
public:
  virtual ~IDelaunay2d() = default;

  void SetOrigin(const DTVertex2f& origin) { mOrigin = origin; }
  DTVertex2f GetOrigin() const noexcept { return mOrigin; }

  virtual void GetGtsInfo(std::vector<DTVertex2f>& vertices,
    std::vector<DTEdge>& edges, std::vector<DTTriangle>& tris) = 0;
  virtual void GetPlyInfo(std::vector<DTVertex3f>& vertices, 
    std::vector<DTTriangle>& tris) = 0;

  //  \return Number of bytes used by the constructed TIN
  virtual size_t GetNumBytes() const = 0;

protected:
  DTVertex2f mOrigin{};

  // estimated statistics
  mutable int mNumV{};  // vertices
  mutable int mNumE{};  // edges
  mutable int mNumT{};  // triangles
  mutable int mNumP{};  // voronoi polygons
};

using UPIDelaunay2d = std::unique_ptr<IDelaunay2d>;
using SPIDelaunay2d = std::shared_ptr<IDelaunay2d>;
}
