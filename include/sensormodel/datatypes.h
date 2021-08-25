/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/exsensormodel.h"

namespace earthxpert {
// index of normalization factors: offset and scale
enum NormalFactorIndex {
  eLINE = 0,   // line 
  eSAMP = 1,   // sample 
  eLAT = 2,    // latitude 
  eLON = 3,    // longitude 
  eHGT = 4,    // height 

  eOLINE = eLINE,  // line offset
  eOSAMP = eSAMP,  // sample offset
  eOLAT = eLAT,    // latitude offset
  eOLON = eLON,    // longitude offset
  eOHGT = eHGT,    // height offset

  eSLINE = 5,   // line scale
  eSSAMP = 6,   // sample scale
  eSLAT = 7,    // latitude scale
  eSLON = 8,    // longitude scale
  eSHGT = 9,    // height scale

  NumOffFact = 5,
  NumSclFact = 5,
  NumFact = 10  // number of factors
};
}
