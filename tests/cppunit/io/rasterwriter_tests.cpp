/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rasterutils.h"
#include "core/utils/rasterlayout.h"
#include "io/raster/rasternullwriter.h"
#include "io/raster/rasterwriter.h"
#include <filesystem>
#include <omp.h>

using namespace earthxpert;
namespace fs = std::filesystem;

/**
 *  \brief compare image contents including image dimension, pixel values, etc.
 *
 *  \param [in] imgFile the original image
 *  \param [in] imgFileT the test image
 *  \return Returns true if the original and the test images are exactly same.
 */
static bool IsImageEqual(const std::string& imgFile, const std::string& imgFileT) {
  RasterReader reader(imgFile);
  auto img = reader.ReadRaster(RasterType::eCVMAT);

  RasterReader readerT(imgFileT);
  auto imgT = readerT.ReadRaster(RasterType::eCVMAT);

  const auto cvimg = dynamic_cast<CvRaster*>(img.get());
  const auto cvimgT = dynamic_cast<CvRaster*>(imgT.get());

  const auto& mat = cvimg->GetCvMat();
  const auto& matT = cvimgT->GetCvMat();

  // \todo

  return true;
}

/**
 *  TestCase 0: null writer
 */
TEST(rasterwriter, NullWriter) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterNullReader reader(imgFile);

  RasterLayout rasterLayout(10, 10, 1, 1);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_NullRaster.tif"));
  RasterNullWriter writer(imgFileT, "", 10, 10, GDT_Unknown, 0);

  // pre-allocate buffer used by all windows
  std::unique_ptr<uchar[]> buffer{ new uchar[1] };

  for (auto& rw : rasterWins) {
    CvRaster raster(rw.numCols, rw.numRows, CV_8UC3, rw.colOffset, rw.rowOffset,
      0, 0, MarginKind::eCONSTANT, buffer.get());
    reader.ReadRaster(&raster);

    auto [numCols, numRows] = raster.GetSize();
    ASSERT_EQ(numCols, rw.numCols);
    ASSERT_EQ(numRows, rw.numRows);

    writer.WriteRaster(&raster);
  }

  ASSERT_FALSE(fs::exists(fs::path(imgFileT))) << imgFileT;
}

/**
 *  \brief cv::Mat test cases
 *
 *  TestCase 1: ortho of three bands in RGB with margins
 */
TEST(rasterwriter, CvRaster) {
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

  ASSERT_TRUE(fs::exists(fs::path(imgFileT))) << imgFileT;
}

/**
 *  \brief cv::Mat test cases OMP
 *
 *  TestCase 2: ortho of three bands in RGB with margins
 */
TEST(rasterwriter, CvRasterOMP) {
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

  ASSERT_TRUE(fs::exists(fs::path(imgFileT))) << imgFileT;
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase 3: ortho of three bands in RGB with margins
 */
TEST(rasterwriter, GdalRaster) {
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
  RasterLayout layout(fileNumCols, fileNumRows, winNumCols, winNumRows);
  auto& rasterWins = layout.GetRasterWindows();

  ASSERT_EQ(9, rasterWins.size());

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

  ASSERT_TRUE(fs::exists(fs::path(imgFileT))) << imgFileT;
}

/**
 *  \brief GDAL test cases OMP
 *
 *  TestCase 4: ortho of three bands in RGB with margins
 */
TEST(rasterwriter, GdalRasterOMP) {
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

  ASSERT_EQ(625, rasterWins.size());

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

  ASSERT_TRUE(fs::exists(fs::path(imgFileT))) << imgFileT;
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase: compare output image files with original images
 */
TEST(rasterwriter, Equality) {
  // TestCase 1
  ASSERT_TRUE(IsImageEqual(TestConfig::DataPath("Imagery/image_small.tif"),
    TestConfig::OutputPath("image_small_t_CvRaster.tif"))) << "image_small_t_CvRaster";
  // TestCase 2
  ASSERT_TRUE(IsImageEqual(TestConfig::DataPath("Imagery/image_small.tif"),
    TestConfig::OutputPath("image_small_t_CvRasterOMP.tif"))) << "image_small_t_CvRasterOMP";
  // TestCase 3
  ASSERT_TRUE(IsImageEqual(TestConfig::DataPath("Imagery/image_small.tif"),
    TestConfig::OutputPath("image_small_t_GdalRaster.tif"))) << "image_small_t_GdalRaster";
  // TestCase 4
  ASSERT_TRUE(IsImageEqual(TestConfig::DataPath("Imagery/image_small.tif"),
    TestConfig::OutputPath("image_small_t_GdalRasterOMP.tif"))) << "image_small_t_GdalRasterOMP";
}
