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
  Initialize();
}

/**
 * constructor.
 *
 * \param [in] wkt the WKT
 */
OgrSrs::OgrSrs(const std::string& wkt) {
  mSrs.reset(new OGRSpatialRef);
  mSrs->importFromWkt(wkt.c_str());
  Initialize();
}

/**
 *  \remark With GDAL 2, the axis mappings are X -> lon, Y -> lat.
 *  Starting with GDAL 3, the axes are mapped according to the WKT definition.
 *  That is, the mappings are X -> lat, Y -> lon. To maintain the backward compatibility,
 *  set the stategy OAMS_TRADITIONAL_GIS_ORDER here.
 *
 *  \todo use the new WKT definition for GDAL 3
 *  \question what to do when mixing GDAL 2 & 3 with 3rd-party
 */
void OgrSrs::Initialize() {
#if 3 <= GDAL_VERSION_MAJOR
  mSrs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif
}
