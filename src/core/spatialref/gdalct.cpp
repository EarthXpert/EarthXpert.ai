/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/gdalct.h"

#include <omp.h>

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] srcSrs the source SRS
 * \param [in] tgtSrs the target SRS
 */
GdalCT::GdalCT(const GdalSRS* srcSrs, const GdalSRS* tgtSrs) {

}

GdalCT::GdalCT(OGRSpatialReference* srcSrs,
  OGRSpatialReference* tgtSrs) {
  Initialize(srcSrs, tgtSrs);
}

/**
 * constructor.
 *
 * \param [in] srcWkt the source WKT
 * \param [in] tgtWkt the target WKT
 *
 *  \remark With GDAL 2, the axis mappings are X -> lon, Y -> lat. 
 *  Starting with GDAL 3, the axes are mapped according to the WKT definition.
 *  That is, the mappings are X -> lat, Y -> lon. To maintain the backward compatibility,
 *  set the stategy OAMS_TRADITIONAL_GIS_ORDER here.
 */
GdalCT::GdalCT(const std::string& srcWkt, const std::string& tgtWkt) {
    auto srcSrs = new OGRSpatialReference(srcWkt.c_str());
    auto tgtSrs = new OGRSpatialReference(tgtWkt.c_str());

#if 3 <= GDAL_VERSION_MAJOR
    srcSrs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    dstSrs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

    Initialize(srcSrs, tgtSrs);
}

/**
 *  \remark GdalCT owns the objects of OGRSpatialReference?
 */
void GdalCT::Initialize(OGRSpatialReference* srcSrs,
  OGRSpatialReference* tgtSrs) {
  mCT.reset(OGRCreateCoordinateTransformation(srcSrs, tgtSrs));
}
