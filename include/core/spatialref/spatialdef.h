/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gdal/exgdal.h"

namespace earthxpert {
//
enum SrsType {
  eST_UNKNOWN = -1,
  eST_GEO,     // geodetic
  eST_LOCAL,   // Cartisian
  eST_MAP      // projected
};

enum SrsBackend {
  eSB_CCS,     // MPS CCS
  eSB_OGR,     // GDAL OGR
  eSB_PROJ     // PROJ.4
};

//
struct SrsInfo {
  SrsBackend backend;
};

//
struct CtInfo {
  SrsBackend backend;
  SrsInfo srcSrs;
  SrsInfo tgtSrs;
};
}
