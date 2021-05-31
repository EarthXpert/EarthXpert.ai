/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include <cfloat>
#include <string>

namespace earthxpert {
//
struct Las2GtsOptions {
  std::string backend;
  std::string lasf;
  std::string gtsf;
  size_t memSize;  // [unit: bytes]
};

struct CvOptions {

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

void EXUTILS_EXPORTS Las2Gts(const std::string& lasFile, 
  const std::string& gtsFile, const CvOptions& cvOpt);
void EXUTILS_EXPORTS Las2Gts(const std::string& lasFile, 
  const std::string& gtsFile, const GtsOptions& gtsOpt);

void EXUTILS_EXPORTS Las2Ply(const std::string& lasFile,
  const std::string& plyFile, const CvOptions& cvOpt);
}
