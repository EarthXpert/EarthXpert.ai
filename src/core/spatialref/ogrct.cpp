/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/ogrct.h"

#include <omp.h>

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] src the source SRS
 * \param [in] tgt the target SRS
 */
OgrCt::OgrCt(const OgrSrs* src, const OgrSrs* tgt) {
  Initialize(src->mSrs.get(), tgt->mSrs.get());
}

OgrCt::OgrCt(OGRSpatialRef* src, OGRSpatialRef* tgt) {
  Initialize(src, tgt);
}

/**
 * constructor.
 *
 * \param [in] srcEpsg the source EPSG code
 * \param [in] tgtEpsg the target EPSG code
 */
OgrCt::OgrCt(const int srcEpsg, const int tgtEpsg) {
  std::unique_ptr<OGRSpatialRef> src{ new OGRSpatialRef };
  src->importFromEPSG(srcEpsg);
  std::unique_ptr<OGRSpatialRef> tgt{ new OGRSpatialRef };
  tgt->importFromEPSG(tgtEpsg);

  Initialize(src.get(), tgt.get());
}

/**
 * constructor.
 *
 * \param [in] srcWkt the source WKT
 * \param [in] tgtWkt the target WKT
 */
OgrCt::OgrCt(const std::string& srcWkt, const std::string& tgtWkt) {
  std::unique_ptr<OGRSpatialRef> src{ new OGRSpatialRef };
  src->importFromWkt(srcWkt.c_str());
  std::unique_ptr<OGRSpatialRef> tgt{ new OGRSpatialRef };
  tgt->importFromWkt(tgtWkt.c_str());

  Initialize(src.get(), tgt.get());
}

/**
 *  \remark OgrCt owns the objects of OGRSpatialRef?
 *
 *  \remark With GDAL 2, the axis mappings are X -> lon, Y -> lat. 
 *  Starting with GDAL 3, the axes are mapped according to the WKT definition.
 *  That is, the mappings are X -> lat, Y -> lon. To maintain the backward compatibility,
 *  set the stategy OAMS_TRADITIONAL_GIS_ORDER here.
 *
 *  \todo use the new WKT definition for GDAL 2
 *  \question what to do when mixing GDAL 2 & 3 with 3rd-party
 */
void OgrCt::Initialize(OGRSpatialRef* src, OGRSpatialRef* tgt) {
#if 3 <= GDAL_VERSION_MAJOR
  src->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  tgt->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

  mCt.reset(OGRCreateCT(src, tgt));
}
