/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/sltbench/sltbench_main_gdal.h"
#include "testconfig.h"

#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/utils.h"
#include "io/raster/rasterreader.h"

using namespace watxtract;

/**
 *  \brief cv::Mat test cases
 *
 *  TestCase 1: ortho of three bands in RGB with no margins
 */
void RasterReader_ORTHOGF1MS_T1() {
  const std::string imgFile(TestConfig::DataPath("Imagery/Satellite/ortho/GF1_PMS1_E112.8_N23.3_20180113_L1A0002927089-MSS1_ORTHO_PSH.tif"));
  RasterReader reader(imgFile);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eCVMAT);
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase 2: ortho of three bands in RGB with no margins
 */
void RasterReader_ORTHOGF1MS_T2() {
  const std::string imgFile(TestConfig::DataPath("Imagery/Satellite/ortho/GF1_PMS1_E112.8_N23.3_20180113_L1A0002927089-MSS1_ORTHO_PSH.tif"));
  RasterReader reader(imgFile);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eGDAL);
}

SLTBENCH_FUNCTION(RasterReader_ORTHOGF1MS_T1); 
SLTBENCH_FUNCTION(RasterReader_ORTHOGF1MS_T2);
