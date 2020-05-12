/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rd_party/gdal/wxgdal.h"
#include <stdexcept>

namespace watxtract {
/**
 *  \brief Initialize GDAL library
 */
void GDALInitialize() {
  CPLSetConfigOption("GDAL_CACHEMAX", "256");
  GDALAllRegister();
}

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
} // namespace watxtract