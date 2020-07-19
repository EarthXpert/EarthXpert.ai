/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/icraftpolicy.h"

namespace earthxpert {
/**
 *  The class AircraftPolicy implements behaviours of aircrafts for line-scanning sensors.
 */
class EXSENSORMODEL_EXPORTS AircraftPolicy : public ICraftPolicy {
public:
  ~AircraftPolicy() = default;
};

/**
 *  The class SpacecraftPolicy implements behaviours of spacecrafts for line-scanning sensors.
 */
class EXSENSORMODEL_EXPORTS SpacecraftPolicy : public ICraftPolicy {
public:
  ~SpacecraftPolicy() = default;
};
} // namespace earthxpert
