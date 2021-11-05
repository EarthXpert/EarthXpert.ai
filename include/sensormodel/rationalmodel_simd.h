/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/math/interpcube.h"
#include "sensormodel/datatypes.h"
#include "sensormodel/sensormodel.h"

#include <array>

namespace earthxpert {
class EXSENSORMODEL_EXPORTS RationalModel_Simd final : public SensorModel {
public:
  RationalModel_Simd();
};
}