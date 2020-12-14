/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/ogrsrs.h"

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] epsg the EPSG code
 */
OgrSrs::OgrSrs(int epsg) {
  mSrs.reset(new OGRSpatialRef);
  mSrs->importFromEPSG(epsg);
}

/**
 * constructor.
 *
 * \param [in] wkt the WKT
 */
OgrSrs::OgrSrs(const std::string& wkt) {
  mSrs.reset(new OGRSpatialRef);
  mSrs->importFromWkt(wkt.c_str());
}
