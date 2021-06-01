/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "utils/delaunay.h"
#include <cfloat>
#include <string>

namespace earthxpert {
void EXUTILS_EXPORTS Las2Gts(const std::string& lasFile, 
  const std::string& gtsFile, const CvOptions& cvOpt);
void EXUTILS_EXPORTS Las2Ply(const std::string& lasFile,
  const std::string& plyFile, const CvOptions& cvOpt);

void EXUTILS_EXPORTS Las2Gts(const std::string& lasFile,
  const std::string& gtsFile, const GtsOptions& gtsOpt);
void EXUTILS_EXPORTS Las2Ply(const std::string& lasFile,
  const std::string& plyFile, const GtsOptions& gtsOpt);
}
