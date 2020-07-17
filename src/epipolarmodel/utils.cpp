/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/utils.h"
#include "epipolarmodel/epipolarmodel.h"

namespace earthxpert {
std::string ToString(const EpipolarRect* epiRect) {
  std::string str;
  return str;
}

std::string ToString(const GeoEpipolarModel* epiModel) {
  auto epiRect = epiModel->GetEpiRect();
  std::string str;
  return str;
}
}