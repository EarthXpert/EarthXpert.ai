/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/ex3rd_party.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

namespace earthxpert {
void WX3RDPARTY_EXPORTS GDALInitialize();
WX3RDPARTY_EXPORTS GDALDataset *CreateRasterDataset(const std::string& imgFile,
  const std::string format, int numCols, int numRows,
  GDALDataType dataType, int numBands);

// using SpatialRefSys          = OGRSpatialReference;
using SpatialReferenceSystem = OGRSpatialReference;

// using SpatialCoordTrans          = OGRCoordinateTransformation;
using SpatialCoordinateTransform = OGRCoordinateTransformation;

inline bool operator==(const SpatialReferenceSystem &lhs,
                       const SpatialReferenceSystem &rhs) {
  return lhs.IsSame(&rhs);
}

inline bool operator!=(const SpatialReferenceSystem &lhs,
                       const SpatialReferenceSystem &rhs) {
  return !(operator==(lhs, rhs));
}
} // namespace earthxpert
