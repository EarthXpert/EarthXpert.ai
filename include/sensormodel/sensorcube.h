/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"
#include "sensormodel/datatypes.h"

#include <array>
#include <memory>

namespace earthxpert {
/**
 *  \brief the class SensorCube creates a 3d cube of image positions and
 *  corresponding ground positions at multiple elevation layers.
 */
class EXSENSORMODEL_EXPORTS SensorCube {
public:
  ~SensorCube() = default;

  int CreateCube();

private:
  std::array<double, NumFact> mNormalFators{}; // normalization factors including offsets & scales
};
}
