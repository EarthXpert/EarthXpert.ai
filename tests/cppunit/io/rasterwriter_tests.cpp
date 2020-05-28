/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/utils.h"
#include "core/utils/rasterlayout.h"
#include "io/raster/rasterwriter.h"
#include <omp.h>

using namespace earthxpert;

/**
 *  \brief cv::Mat test cases
 *
 *  TestCase 1: ortho of three bands in RGB with margins
 */
TEST(RasterWriter, CvRaster) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  const int marginCols{ 20 };
  const int marginRows{ 20 };
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto gdalDT = reader.GetDataType();
  auto numBands = reader.GetNumBands();
  auto cvDT = ConvertPixelType(gdalDT, numBands);
  std::string format = reader.GetFileFormat();

  auto srs = reader.GetCoordinateSystem();
  auto geocoding = reader.GetGeocoding();

  const int winNumCols{ 2000 };
  const int winNumRows{ 2000 };
  RasterLayout rasterLayout(fileNumCols, fileNumRows, winNumCols, winNumRows);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_CvRaster.tif"));
  RasterWriter writer(imgFileT, format, fileNumCols, fileNumRows, gdalDT, numBands);

  writer.SetCoordinateSystem(srs.get());
  writer.SetGeocoding(geocoding.get());

  // pre-allocate buffer used by all windows
  const auto bufNumBytes = reader.GetRasterBufferNumBytes(winNumCols, winNumRows);
  std::unique_ptr<uchar[]> buffer{ new uchar[bufNumBytes] };

  for (auto& rw : rasterWins) {  
    CvRaster raster(rw.numCols, rw.numRows, cvDT, rw.colOffset, rw.rowOffset,
      marginCols, marginRows, MarginKind::eCONSTANT, buffer.get());
    reader.ReadRaster(&raster);

    auto [numCols, numRows] = raster.GetSize();
    ASSERT_EQ(numCols, rw.numCols);
    ASSERT_EQ(numRows, rw.numRows);

    auto [bufNumCols, bufNumRows] = raster.GetBufferSize();
    ASSERT_EQ(bufNumCols, rw.numCols + 40);
    ASSERT_EQ(bufNumRows, rw.numRows + 40);

    writer.WriteRaster(&raster);
  }
}

/**
 *  \brief cv::Mat test cases OMP
 *
 *  TestCase 2: ortho of three bands in RGB with margins
 */
TEST(RasterWriter, CvRasterOMP) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  const int marginCols{ 20 };
  const int marginRows{ 20 };
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto gdalDT = reader.GetDataType();
  auto numBands = reader.GetNumBands();
  auto cvDT = ConvertPixelType(gdalDT, numBands);
  std::string format = reader.GetFileFormat();

  auto srs = reader.GetCoordinateSystem();
  auto geocoding = reader.GetGeocoding();

  const int winNumCols{ 200 };
  const int winNumRows{ 200 };
  RasterLayout rasterLayout(fileNumCols, fileNumRows, winNumCols, winNumRows);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_CvRasterOMP.tif"));
  RasterWriter writer(imgFileT, format, fileNumCols, fileNumRows, gdalDT, numBands);

  writer.SetCoordinateSystem(srs.get());
  writer.SetGeocoding(geocoding.get());

#pragma omp parallel for
  for (int64 i = 0; i < rasterWins.size(); ++i) {  // auto& rw : rasterWins
    auto& rw = rasterWins[i];
    CvRaster raster(rw.numCols, rw.numRows, cvDT, rw.colOffset, rw.rowOffset,
      marginCols, marginRows, MarginKind::eCONSTANT);
    reader.ReadRaster(&raster);

    writer.WriteRaster(&raster);
  }
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase 3: ortho of three bands in RGB with margins
 */
TEST(RasterWriter, GdalRaster) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  const int marginCols{ 20 };
  const int marginRows{ 20 };
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto gdalDT = reader.GetDataType();
  auto numBands = reader.GetNumBands();
  std::string format = reader.GetFileFormat();

  auto srs = reader.GetCoordinateSystem();
  auto geocoding = reader.GetGeocoding();

  const int winNumCols{ 2000 };
  const int winNumRows{ 2000 };
  RasterLayout rasterLayout(fileNumCols, fileNumRows, winNumCols, winNumRows);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_GdalRaster.tif"));
  RasterWriter writer(imgFileT, format, fileNumCols, fileNumRows, gdalDT, numBands);

  writer.SetCoordinateSystem(srs.get());
  writer.SetGeocoding(geocoding.get());

  // pre-allocate buffer used by all windows
  const auto bufNumBytes = reader.GetRasterBufferNumBytes(winNumCols, winNumRows);
  std::unique_ptr<uchar[]> buffer{ new uchar[bufNumBytes] };

  for (auto& rw : rasterWins) {
    GdalRaster raster(rw.numCols, rw.numRows, gdalDT, numBands, rw.colOffset, rw.rowOffset,
      marginCols, marginRows, MarginKind::eCONSTANT, buffer.get());
    reader.ReadRaster(&raster);

    auto [numCols, numRows] = raster.GetSize();
    ASSERT_EQ(numCols, rw.numCols);
    ASSERT_EQ(numRows, rw.numRows);

    auto [bufNumCols, bufNumRows] = raster.GetBufferSize();
    ASSERT_EQ(bufNumCols, rw.numCols + 40);
    ASSERT_EQ(bufNumRows, rw.numRows + 40);

    writer.WriteRaster(&raster);
  }
}

/**
 *  \brief GDAL test cases OMP
 *
 *  TestCase 4: ortho of three bands in RGB with margins
 */
TEST(RasterWriter, GdalRasterOMP) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  const int marginCols{ 20 };
  const int marginRows{ 20 };
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto gdalDT = reader.GetDataType();
  auto numBands = reader.GetNumBands();
  std::string format = reader.GetFileFormat();

  auto srs = reader.GetCoordinateSystem();
  auto geocoding = reader.GetGeocoding();

  const int winNumCols{ 200 };
  const int winNumRows{ 200 };
  RasterLayout rasterLayout(fileNumCols, fileNumRows, winNumCols, winNumRows);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_GdalRasterOMP.tif"));
  RasterWriter writer(imgFileT, format, fileNumCols, fileNumRows, gdalDT, numBands);

  writer.SetCoordinateSystem(srs.get());
  writer.SetGeocoding(geocoding.get());

#pragma omp parallel for
  for (int64 i = 0; i < rasterWins.size(); ++i) {  // auto& rw : rasterWins
    auto& rw = rasterWins[i];
    GdalRaster raster(rw.numCols, rw.numRows, gdalDT, numBands, rw.colOffset, rw.rowOffset,
      marginCols, marginRows, MarginKind::eCONSTANT);
    reader.ReadRaster(&raster);

    writer.WriteRaster(&raster);
  }
}
