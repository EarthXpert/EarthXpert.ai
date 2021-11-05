/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rastertypes.h"

#include <gdal_priv.h>
#include <memory>
#include <vector>

namespace earthxpert {
int EXCORE_EXPORTS ConvertPixelType(GDALDataType gdalType, int numBands);
std::pair<GDALDataType, int> EXCORE_EXPORTS ConvertPixelType(int cvType);

void EXCORE_EXPORTS CopyPixel(const CvRaster* raster, int c, int r, int cp, int rp);
void EXCORE_EXPORTS SetPixel(const CvRaster* raster, int c, int r, const uchar* src);

void EXCORE_EXPORTS FillMargins(CvRaster* const raster, MarginFillDim dim);

std::unique_ptr<CvRaster> EXCORE_EXPORTS 
ConvertRaster(const GdalRaster* gdalRaster);
std::unique_ptr<CvRaster> EXCORE_EXPORTS 
ConvertRaster(const GdalRaster* gdalRaster, int band);
std::vector<std::unique_ptr<CvRaster>> EXCORE_EXPORTS 
ConvertRaster(const GdalRaster* gdalRaster, const std::vector<int>& bands);
std::unique_ptr<GdalRaster> EXCORE_EXPORTS 
ConvertRaster(const CvRaster* cvRaster);
std::unique_ptr<GdalRaster> EXCORE_EXPORTS 
ConvertRaster(const CvRaster* cvRaster, int band);
}
