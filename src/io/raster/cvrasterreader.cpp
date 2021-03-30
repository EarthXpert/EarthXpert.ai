/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/cvrasterreader.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rasterutils.h"
#include "core/exceptions/exexcept.h"
#include "core/spatialref/geocoding.h"
#include "core/utils/typehelper.h"

#include <numeric>

using namespace earthxpert;

/**
 *  \brief Construct reader object.
 *
 *  \param [in] imageFile full path of the image file
 *  \param [in] bands an array of 1-based band indices
 */
CvRasterReader::CvRasterReader(const std::string& imageFile,
  const std::vector<int>& bands) : mBands(bands) {
}

CvRasterReader::CvRasterReader(const fs::path& imageFile, const std::vector<int>& bands)
  : CvRasterReader(imageFile.string(), bands) {}
