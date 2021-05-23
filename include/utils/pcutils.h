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
}
