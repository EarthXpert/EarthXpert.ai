/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gdal/exgdal.h"

namespace earthxpert {
//
enum SrsType {
  eUNKNOWN = -1,
  eGEO,     // geodetic
  eLOCAL,
  eMAP      // projected
};
}
