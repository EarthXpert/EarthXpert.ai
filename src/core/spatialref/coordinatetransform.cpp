/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/coordinatetransform.h"

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] srcSrs the source SRS
 * \param [in] tgtSrs the target SRS
 */
CoordinateTransform::CoordinateTransform(OGRSpatialReference* srcSrs,
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
CoordinateTransform::CoordinateTransform(const std::string& srcWkt, 
  const std::string& tgtWkt) {
    OGRSpatialReference srcSrs(srcWkt.c_str());
    OGRSpatialReference tgtSrs(tgtWkt.c_str());

#if 3 <= GDAL_VERSION_MAJOR
    srcSrs.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    dstSrs.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

    Initialize(&srcSrs, &tgtSrs);
}

void CoordinateTransform::Initialize(OGRSpatialReference* srcSrs,
  OGRSpatialReference* tgtSrs) {
  mCT.reset(OGRCreateCoordinateTransformation(srcSrs, tgtSrs));
}

/**
 *  \brief Transform an array of points in-place.
 *
 *  \param [in] points an array of 3D points
 *  \return Returns if the transformation was successful, otherwise false.
 */
bool CoordinateTransform::Transform(std::vector<Point3d>& points) {
  bool ret{ true };
  for (auto& point : points) {
    ret = ret && Transform(point);
  }

  return ret;
}
