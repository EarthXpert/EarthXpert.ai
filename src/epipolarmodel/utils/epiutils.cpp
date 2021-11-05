/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/utils/epiutils.h"
#include "epipolarmodel/epipolarmodel.h"
#include <sstream>

namespace earthxpert {
std::string ToString(const EpipolarRect& epiRect) {
  std::stringstream ss;
  return ss.str();
}

std::string ToString(const GeoEpipolarModel& epiModel) {
  auto epiRect = epiModel.GetEpiRect();
  std::stringstream ss;
  return ss.str();
}
}
