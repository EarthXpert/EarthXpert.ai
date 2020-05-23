/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/types.h"

#include "gdal_priv.h"
#include <memory>
#include <vector>

namespace earthxpert {
int WXCORE_EXPORTS ConvertPixelType(GDALDataType gdalType, int numBands);
std::pair<GDALDataType, int> WXCORE_EXPORTS ConvertPixelType(int cvType);

void WXCORE_EXPORTS CopyPixel(const CvRaster* raster, int c, int r, int cp, int rp);
void WXCORE_EXPORTS SetPixel(const CvRaster* raster, int c, int r, const uchar* src);

void WXCORE_EXPORTS FillMargins(CvRaster* const raster, MarginFillDim dim);

std::unique_ptr<CvRaster> WXCORE_EXPORTS ConvertRaster(const GdalRaster* gdalRaster);
std::unique_ptr<CvRaster> WXCORE_EXPORTS ConvertRaster(const GdalRaster* gdalRaster, int band);
std::vector<std::unique_ptr<CvRaster>> WXCORE_EXPORTS ConvertRaster(const GdalRaster* gdalRaster, const std::vector<int>& bands);
std::unique_ptr<GdalRaster> WXCORE_EXPORTS ConvertRaster(const CvRaster* gdalRaster);
}
