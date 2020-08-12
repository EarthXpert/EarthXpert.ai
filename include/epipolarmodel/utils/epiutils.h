/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/exepipolarmodel.h"
#include <string>

namespace earthxpert {
class EpipolarRect;

std::string EXEPIPOLARMODEL_EXPORTS ToString(const EpipolarRect* epiRect);
//std::string EXEPIPOLARMODEL_EXPORTS ToString(const GeoEpipolarModel* epiModel);
}
