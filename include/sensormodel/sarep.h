/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once
#include "sensormodel/exsensormodel.h"
#include <chrono>

namespace earthxpert {
/**
 *  The class SarEP defines ephemeris parameters for exterior orientation
 *  of SAR scanner.
 *
 *  \detail
 */
class EXSENSORMODEL_EXPORTS SarEP {
public:
  /**
   * Constructor
   */
  SarEP();

  ~SarEP() {}

private:
  schrono::time_point<schrono::steady_clock> mBase;

  double mTimeInterval{};
  std::vector<Point3d> mPos;  // XYZ positions of antena phase center
  std::vector<Point3d> mVel;  // VxVyVz velocities of antena phase center
};
} // namespace earthxpert
