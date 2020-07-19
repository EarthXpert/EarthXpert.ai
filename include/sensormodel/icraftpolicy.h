/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"

namespace earthxpert {
/**
 *  The interface ICraftPolicy defines behaviours of crafts for line-scanning sensors.
 */
class EXSENSORMODEL_EXPORTS ICraftPolicy {
public:
  ~ICraftPolicy() = default;
};
} // namespace earthxpert
