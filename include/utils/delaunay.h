/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "3rdparty/gts/delaunay2d.h"
#include "3rdparty/opencv/delaunay2d.h"

#include <cfloat>
#include <memory>
#include <string>

namespace earthxpert {
//
struct DelaunayOptions {
  std::string backend;
  std::string lasf;
  std::string tinf;
  size_t memSize;  // [unit: bytes]
};

struct CvOptions {
  bool checkDelaunay{ true };
  bool verbose{ true };
};

struct GtsOptions {
  double quality{ 0.0 };
  double area{ DBL_MAX };
  int steiner_max{ -1 };
  bool keepHull{ true };
  //bool removeHoles{false};
  bool conform{ true };
  bool refine{ true };
  bool checkDelaunay{ true };
  bool verbose{ true };
};

std::unique_ptr<CvDelaunay2d> EXUTILS_EXPORTS CvDelaunay(
  const std::string& lasFile, const CvOptions& cvOpt);
EXUTILS_EXPORTS GtsSurface* GtsDelaunay(const std::string& lasFile, 
  const GtsOptions& gtsOpt);
}
