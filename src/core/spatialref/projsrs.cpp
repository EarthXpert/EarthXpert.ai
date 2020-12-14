/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/projsrs.h"

using namespace earthxpert;

/**
 * constructor.
 *
 * \param [in] wkt the WKT
 */
ProjSrs::ProjSrs(const std::string& wkt) {
  mSRS.reset(proj_create(PJ_DEFAULT_CTX, wkt.c_str()));
}
