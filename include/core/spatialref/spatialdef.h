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

inline bool operator==(const SrsInfo& lhs, const SrsInfo& rhs) {
  return false;
}

inline bool operator<(const SrsInfo& lhs, const SrsInfo& rhs) {
  return false;
}

//
struct CtInfo {
  SrsInfo srcSrs;
  SrsInfo tgtSrs;
};

inline bool operator==(const CtInfo& lhs, const CtInfo& rhs) {
  return false;
}

inline bool operator<(const CtInfo& lhs, const CtInfo& rhs) {
  return false;
}
}
