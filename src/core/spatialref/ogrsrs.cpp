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
OgrSRS::OgrSRS(int epsg) {
  mSRS.reset(new OGRSpatialRef);
  mSRS->importFromEPSG(epsg);
}

/**
 * constructor.
 *
 * \param [in] wkt the WKT
 */
OgrSRS::OgrSRS(const std::string& wkt) {
  mSRS.reset(new OGRSpatialRef);
  mSRS->importFromWkt(wkt.c_str());
}
