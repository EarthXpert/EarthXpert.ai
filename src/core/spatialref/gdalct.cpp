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
 * \param [in] src the source SRS
 * \param [in] tgt the target SRS
 */
GdalCT::GdalCT(const GdalSRS* src, const GdalSRS* tgt) {

}

GdalCT::GdalCT(OGRSpatialReference* src, OGRSpatialReference* tgt) {
  Initialize(src, tgt);
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
    auto src = new OGRSpatialReference(srcWkt.c_str());
    auto tgt = new OGRSpatialReference(tgtWkt.c_str());

#if 3 <= GDAL_VERSION_MAJOR
    src->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    tgt->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

    Initialize(src, tgt);
}

/**
 *  \remark GdalCT owns the objects of OGRSpatialReference?
 */
void GdalCT::Initialize(OGRSpatialReference* src,
  OGRSpatialReference* tgt) {
  mCT.reset(OGRCreateCoordinateTransformation(src, tgt));
}
