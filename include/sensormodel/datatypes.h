/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"

namespace earthxpert {
// index of normalization factors
enum NormalizationFactor {
  eOLINE = 0,   // line offset
  eOSAMP = 1,   // sample offset
  eOLAT = 2,    // latitude offset
  eOLON = 3,    // longitude offset
  eOHGT = 4,    // height offset
  eSLINE = 5,   // line scale
  eSSAMP = 6,   // sample scale
  eSLAT = 7,    // latitude scale
  eSLON = 8,    // longitude scale
  eSHGT = 9,    // height scale
  NumFact = 10  // number of factors
};
}
