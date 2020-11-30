/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include "proj.h"
//#include "proj_api.h"   // deprecated API
#include "proj_constants.h"

#include <memory>
#include <tuple>

/**
 *  PROJ_LIB=D:\home\workspace\install\windows\etc\proj
 *
 *  \details 
 *
 *    grid shift files format: 
 *      horizontal: b, dac, gsb, los/las, tif
 *      vertical: gtx, json, tif
 */
namespace earthxpert {
inline std::tuple<double, double, double> 
GetXYZ(const PJ_COORD& pc) {
  return {pc.v[0], pc.v[1], pc.v[2]};
}

// 
struct PJDeleter {
  void operator()(PJ* pj) { proj_destroy(pj); }
};

using UPPJ = std::unique_ptr<PJ, PJDeleter>;
} // namespace earthxpert
