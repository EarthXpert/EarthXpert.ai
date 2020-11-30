/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <gdal.h>
#include <gdal_priv.h>
#include <gdal_utils.h>
#include <ogr_core.h>
#include <ogr_spatialref.h>
#include <string>
#include <vector>

/**
 *  GDAL_DATA=D:\home\workspace\install\windows\etc\gdal
 */
namespace earthxpert {    // \todo ::thirdparty
void EX3RDPARTY_EXPORTS GDALInitialize();

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

// field information
struct FieldInfo {
  OGRFieldType type{ OFTInteger };
  std::string name;
};

// layer information
struct LayerInfo {
  OGRwkbGeometryType type{ wkbUnknown };
  std::string name;
  std::vector<FieldInfo> fields;
};

namespace thirdparty {
EX3RDPARTY_EXPORTS GDALDataset* CreateRasterDataset(const std::string& imgFile,
  const std::string format, int numCols, int numRows, GDALDataType dataType, int numBands);
EX3RDPARTY_EXPORTS GDALDataset* CreateVectorDataset(const std::string& vecFile,
  const std::string format, const std::vector<LayerInfo>& layersInfo);
}

using UPGDALDataset = GDALDatasetUniquePtr;
using UPSpatialRefSys = std::unique_ptr<SpatialReferenceSystem>;
using SPSpatialRefSys = std::shared_ptr<SpatialReferenceSystem>;
using UPSpatialCoordTrans = std::unique_ptr<SpatialCoordinateTransform>;
using SPSpatialCoordTrans = std::shared_ptr<SpatialCoordinateTransform>;
} // namespace earthxpert::thirdparty
