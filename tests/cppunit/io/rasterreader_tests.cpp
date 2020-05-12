/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main_gdal.h"
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
TEST(RasterReader, ORTHOGF1MS_T1) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eCVMAT);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2000);
  ASSERT_EQ(bufNumRows, 2000);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 3);

  auto cvRaster = dynamic_cast<CvRaster*>(raster.get());

  const auto dataType = cvRaster->GetDataType();
  ASSERT_EQ(dataType, CV_8UC3);

  const auto pix1a = cvRaster->GetPixel<cv::Vec3b>(1000, 1000);
  cv::Mat mat = cvRaster->GetCvMat();
  const auto& pix1b = mat.at<cv::Vec3b>(1000, 1000);
  ASSERT_EQ(pix1b[0], (*pix1a)[0]);
  ASSERT_EQ(pix1b[1], (*pix1a)[1]);
  ASSERT_EQ(pix1b[2], (*pix1a)[2]);
}

/**
 *  TestCase 2: ortho of three bands in RGB with margins
 */
TEST(RasterReader, ORTHOGF1MS_T2a) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  // margins within image file
  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eCVMAT);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2040);
  ASSERT_EQ(bufNumRows, 2040);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 3);

  auto cvRaster = dynamic_cast<CvRaster*>(raster.get());
  const auto dataType = cvRaster->GetDataType();
  ASSERT_EQ(dataType, CV_8UC3);

  const auto pix1a = cvRaster->GetPixel<cv::Vec3b>(1000, 1000);
  cv::Mat mat = cvRaster->GetCvMat();
  const auto& pix1b = mat.at<cv::Vec3b>(1020, 1020);
  ASSERT_EQ(pix1b[0], (*pix1a)[0]);
  ASSERT_EQ(pix1b[1], (*pix1a)[1]);
  ASSERT_EQ(pix1b[2], (*pix1a)[2]);
}

/**
 *  TestCase 2: aerial of three bands in RGB with margins
 */
TEST(RasterReader, AERIALMS_T2b) {
  const std::string imgFile(TestConfig::DataPath("Imagery/Aerial/Graz25/images/Hi-RGB_02152.tif"));
  RasterReader reader(imgFile);
  reader.SetMargins(20, 20, MarginKind::eREFLECTION);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 9420);
  ASSERT_EQ(fileNumRows, 14430);

  // margins partly outside image file
  auto raster = reader.ReadRaster(0, 0, 2000, 2000, RasterType::eCVMAT);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2040);
  ASSERT_EQ(bufNumRows, 2040);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 3);

  auto cvRaster = dynamic_cast<CvRaster*>(raster.get());
  const auto dataType = cvRaster->GetDataType();
  ASSERT_EQ(dataType, CV_8UC3);

  const auto pix1a = cvRaster->GetPixel<cv::Vec3b>(1000, 1000);
  ASSERT_EQ((*pix1a)[0], 101);
  ASSERT_EQ((*pix1a)[1], 94);
  ASSERT_EQ((*pix1a)[2], 87);

  cv::Mat mat = cvRaster->GetCvMat();
  auto pix1b = mat.at<cv::Vec3b>(1020, 1020);
  ASSERT_EQ(pix1b[0], 101);
  ASSERT_EQ(pix1b[1], 94);
  ASSERT_EQ(pix1b[2], 87);
}

/**
 *  TestCase 3: ortho of 2nd band with no margins
 */
TEST(RasterReader, ORTHOGF1MS_T3) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile, {2});

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eCVMAT);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 1);

  auto cvRaster = dynamic_cast<CvRaster*>(raster.get());

  const auto dataType = cvRaster->GetDataType();
  ASSERT_EQ(dataType, CV_8UC1);

  const auto pix1a = cvRaster->GetPixel<uchar>(1000, 1000);
  cv::Mat mat = cvRaster->GetCvMat();
  const auto pix1b = mat.at<uchar>(1000, 1000);
  ASSERT_EQ(pix1b, *pix1a);
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase 4: ortho of three bands in RGB with no margins
 */
TEST(RasterReader, ORTHOGF1MS_T4) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eGDAL);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2000);
  ASSERT_EQ(bufNumRows, 2000);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 3);

  auto gdalRaster = dynamic_cast<GdalRaster*>(raster.get());

  const auto dataType = gdalRaster->GetDataType();
  ASSERT_EQ(dataType, GDT_Byte);

  const auto pix = gdalRaster->GetPixel<uchar>(1000, 1000);
  ASSERT_FALSE(bool(pix)) << "GetPixel is not implemented for multiple bands";

  // test conversion
  auto cvRaster = ConvertRaster(gdalRaster);
  const auto cvType = cvRaster->GetDataType();
  ASSERT_EQ(cvType, CV_8UC3);

  const auto pix2a = cvRaster->GetPixel<cv::Vec3b>(1000, 1000);
  cv::Mat mat = cvRaster->GetCvMat();
  const auto& pix2b = mat.at<cv::Vec3b>(1000, 1000);
  ASSERT_EQ(pix2b[0], (*pix2a)[0]);
  ASSERT_EQ(pix2b[1], (*pix2a)[1]);
  ASSERT_EQ(pix2b[2], (*pix2a)[2]);
}

/**
 *  TestCase 5: margin kind filling behaviours
 */
TEST(RasterReader, AERIALMS_T5) {
  const std::string imgFile(TestConfig::DataPath("Imagery/Aerial/Graz25/images/Hi-RGB_02152.tif"));
  RasterReader reader(imgFile);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 9420);
  ASSERT_EQ(fileNumRows, 14430);

  // constant
  reader.SetMargins(20, 20, MarginKind::eCONSTANT);
  auto raster = reader.ReadRaster(0, 0, 2000, 2000, RasterType::eCVMAT);
  auto cvRaster = dynamic_cast<CvRaster*>(raster.get());

  const auto pix1a = cvRaster->GetPixel<cv::Vec3b>(1000, -10);
  ASSERT_EQ((*pix1a)[0], 0);
  ASSERT_EQ((*pix1a)[1], 0);
  ASSERT_EQ((*pix1a)[2], 0);

  cv::Mat mat = cvRaster->GetCvMat();
  auto pix1b = mat.at<cv::Vec3b>(10, 1020);
  ASSERT_EQ(pix1b[0], 0);
  ASSERT_EQ(pix1b[1], 0);
  ASSERT_EQ(pix1b[2], 0);

  // reflection
  reader.SetMargins(20, 20, MarginKind::eREFLECTION);
  raster = reader.ReadRaster(fileNumCols/2-1000, 10, 2000, 2000, RasterType::eCVMAT);
  cvRaster = dynamic_cast<CvRaster*>(raster.get());

  const auto pix2a = cvRaster->GetPixel<cv::Vec3b>(1000, -15);
  ASSERT_EQ((*pix2a)[0], 36);
  ASSERT_EQ((*pix2a)[1], 36);
  ASSERT_EQ((*pix2a)[2], 27);

  mat = cvRaster->GetCvMat();
  auto pix2b = mat.at<cv::Vec3b>(5, 1020);
  ASSERT_EQ(pix2b[0], 36);
  ASSERT_EQ(pix2b[1], 36);
  ASSERT_EQ(pix2b[2], 27);

  // replication
  reader.SetMargins(20, 20, MarginKind::eREPLICATION);
  raster = reader.ReadRaster(fileNumCols-2005, fileNumRows-2005, 2000, 2000, RasterType::eCVMAT);
  cvRaster = dynamic_cast<CvRaster*>(raster.get());

  const auto pix3a = cvRaster->GetPixel<cv::Vec3b>(1000, 2010);
  ASSERT_EQ((*pix3a)[0], 134);
  ASSERT_EQ((*pix3a)[1], 132);
  ASSERT_EQ((*pix3a)[2], 129);

  mat = cvRaster->GetCvMat();
  auto pix3b = mat.at<cv::Vec3b>(2030, 1020);
  ASSERT_EQ(pix3b[0], 134);
  ASSERT_EQ(pix3b[1], 132);
  ASSERT_EQ(pix3b[2], 129);
}

/**
 *  \brief GDAL test cases
 *
 *  TestCase 6: ortho of three bands in RGB with margins
 */
TEST(RasterReader, ORTHOGF1MS_T6) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile);
  reader.SetMargins(20, 20, MarginKind::eREPLICATION);

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eGDAL);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2040);
  ASSERT_EQ(bufNumRows, 2040);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 3);

  auto gdalRaster = dynamic_cast<GdalRaster*>(raster.get());

  const auto dataType = gdalRaster->GetDataType();
  ASSERT_EQ(dataType, GDT_Byte);

  const auto pix = gdalRaster->GetPixel<uchar>(1000, 1000);
  ASSERT_FALSE(bool(pix)) << "GetPixel is not implemented for multiple bands";

  // test conversion
  std::vector<int> bands{0, 1, 2};
  auto cvRasters = ConvertRaster(gdalRaster, bands);
  const auto cvType = cvRasters[0]->GetDataType();
  ASSERT_EQ(cvType, CV_8UC1);

  const auto pix2a = cvRasters[0]->GetPixel<uchar>(1000, 1000);
  auto mat0 = cvRasters[0]->GetCvMat();
  const auto pix2b = mat0.at<uchar>(1020, 1020);
  ASSERT_EQ(pix2b, (*pix2a));

  const auto pix3a = cvRasters[1]->GetPixel<uchar>(1000, 1000);
  auto mat1 = cvRasters[1]->GetCvMat();
  const auto pix3b = mat1.at<uchar>(1020, 1020);
  ASSERT_EQ(pix3b, (*pix3a));

  const auto pix4a = cvRasters[2]->GetPixel<uchar>(1000, 1000);
  auto mat2 = cvRasters[2]->GetCvMat();
  const auto pix4b = mat2.at<uchar>(1020, 1020);
  ASSERT_EQ(pix4b, (*pix4a));
}

/**
 *  TestCase 7: ortho of 2nd band in RGB with no margins
 */
TEST(RasterReader, ORTHOGF1MS_T7) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  RasterReader reader(imgFile, { 2 });

  auto [fileNumCols, fileNumRows] = reader.GetSize();
  ASSERT_EQ(fileNumCols, 5000);
  ASSERT_EQ(fileNumRows, 5000);

  auto raster = reader.ReadRaster(fileNumCols / 2 - 1000, fileNumRows / 2 - 1000, 2000, 2000, RasterType::eGDAL);

  auto [numCols, numRows] = raster->GetSize();
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  auto [bufNumCols, bufNumRows] = raster->GetBufferSize();
  ASSERT_EQ(bufNumCols, 2000);
  ASSERT_EQ(bufNumRows, 2000);

  const auto numBands = raster->GetNumBands();
  ASSERT_EQ(numBands, 1);

  auto gdalRaster = dynamic_cast<GdalRaster*>(raster.get());

  const auto dataType = gdalRaster->GetDataType();
  ASSERT_EQ(dataType, GDT_Byte);

  const auto pix = gdalRaster->GetPixel<uchar>(1000, 1000);
  ASSERT_TRUE(bool(pix)) << "only single band is implemented till now";

  // test conversion
  auto cvRaster = ConvertRaster(gdalRaster, 0);
  const auto cvType = cvRaster->GetDataType();
  ASSERT_EQ(cvType, CV_8UC1);

  const auto pix2a = cvRaster->GetPixel<uchar>(1000, 1000);
  ASSERT_EQ(*pix2a, *pix);

  cv::Mat mat = cvRaster->GetCvMat();
  const auto& pix2b = mat.at<uchar>(1000, 1000);
  ASSERT_EQ(pix2b, *pix);
}
