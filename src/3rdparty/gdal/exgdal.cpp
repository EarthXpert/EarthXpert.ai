/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/gdal/exgdal.h"

#include <filesystem>
//#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace earthxpert {
/**
 *  \brief Initialize GDAL library
 */
void GDALInitialize() {
  auto data = fs::canonical(fs::current_path() / "../../etc/gdal");
  //std::cout << "GDAL_DATA = " << data << std::endl;
  CPLSetConfigOption("GDAL_CACHEMAX", "256");
  CPLSetConfigOption("GDAL_DATA", data.string().c_str());
  GDALAllRegister();
}

namespace thirdparty {
/**
 *  \brief create a raster dataset
 *
 *  \param [in] imageFile path and name of image file to create
 *  \param [in] format format of image file
 *  \param [in] numCols number of columns in the image
 *  \param [in] numRows number of rows in the image
 *  \param [in] dataType datatype for the image
 *  \param [in] numBands number of bands in the image
 *  \return Return pointer to GDALDataset if operation succeeds.
 */
GDALDataset *CreateRasterDataset(const std::string& imgFile,
  const std::string format, const int numCols, const int numRows,
  const GDALDataType dataType, const int numBands) {
  auto driver = GetGDALDriverManager()->GetDriverByName(format.c_str());
  if (!driver) {
    throw std::invalid_argument("GDAL does not support given image format");
  }

  auto metaData = driver->GetMetadata();
  if (FALSE == CSLFetchBoolean(metaData, GDAL_DCAP_CREATE, FALSE)) {
    throw std::invalid_argument("GDAL does not support the creation of given image format");
  }

  return driver->Create(imgFile.c_str(), numCols, numRows, numBands, dataType, nullptr);
}

/**
 *  \brief create a vector dataset
 *
 *  \param [in] vecFile path and name of vector data source to create
 *  \param [in] format format of image file
 *  \param [in] layersInfo the vector layers information
 *  \return Return pointer to GDALDataset if operation succeeds.
 */
GDALDataset* CreateVectorDataset(const std::string& vecFile,
  const std::string format, const std::vector<LayerInfo>& layersInfo) {
  auto driver = GetGDALDriverManager()->GetDriverByName(format.c_str());
  if (!driver) {
    throw std::invalid_argument("OGR does not support given vector format");
  }

  auto metaData = driver->GetMetadata();
  if (FALSE == CSLFetchBoolean(metaData, GDAL_DCAP_CREATE, FALSE)) {
    throw std::invalid_argument("OGR does not support the creation of given vector format");
  }

  auto dataset = driver->Create(vecFile.c_str(), 0, 0, 0, GDT_Unknown, nullptr);
  if (!dataset) {
    throw std::invalid_argument("OGR failed to create vector data source");
  }

  for (auto layerInfo : layersInfo) {
    auto layer = dataset->CreateLayer("", nullptr, layerInfo.type, nullptr);
  }

  return dataset;
}
}
} // namespace earthxpert
