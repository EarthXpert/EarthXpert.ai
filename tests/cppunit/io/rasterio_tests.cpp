#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "opencv2/imgproc/imgproc.hpp"

#include "3rd_party/gdal/wxgdal.h"
#include "io/raster/imagereader.h"
#include "io/raster/imagewriter.h"

using namespace watxtract;

TEST(ImageReader, ORTHOGF1MS) {
  const std::string imgFile(TestConfig::DataPath("Imagery/Satellite/ortho/GF1_PMS1_E112.8_N23.3_20180113_L1A0002927089-MSS1_ORTHO_PSH.tif"));
  ImageReader imgReader(imgFile);

  int32_t imgCols, imgRows;
  imgReader.GetSize(imgCols, imgRows);
  ASSERT_EQ(imgCols, 22552);
  ASSERT_EQ(imgRows, 20821);

  auto imgWin = imgReader.ReadRaster(imgCols/2-1000, imgRows/2-1000, 2000, 2000);

  int32_t numCols, numRows;
  imgWin->GetSize(numCols, numRows);
  ASSERT_EQ(numCols, 2000);
  ASSERT_EQ(numRows, 2000);

  const auto numBands = imgWin->GetNumBands();
  ASSERT_EQ(numBands, 3);

  const auto dataType = imgWin->GetDataType();
  ASSERT_EQ(dataType, GDT_UInt16);

  cv::Mat mat = imgWin->GetCVData();
  cvtColor(mat, mat, cv::COLOR_RGB2BGR);

  const auto pix = mat.at<cv::Vec3w>(1000, 1000);
  ASSERT_EQ(pix[0], 323);
  ASSERT_EQ(pix[1], 309);
  ASSERT_EQ(pix[2], 189);
}
/*
TEST(ImageWindow, ImageWindow) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  ImageReader imgReader(imgFile);

  int32_t readCol, readRow;
  imgReader.GetSize(readCol, readRow);
  auto imgWin = imgReader.ReadRaster(0, 0, readCol, readRow);

  // imgWin functions
  int32_t numCol, numRow, numColOffset, numRowOffset, marginX, marginY;
  imgWin->GetMargin(marginX, marginY);
  imgWin->GetOffset(numColOffset, numRowOffset);
  imgWin->GetSize(numCol, numRow);

  int16_t numBand = imgWin->GetNumBands();
  GDALDataType dataType = imgWin->GetDataType();

  cv::Mat singleMat = imgWin->GetChannelData(1);
  cv::Mat mat = imgWin->GetCVData();

  cvtColor(mat, mat, cv::COLOR_BGR2RGB);

  ASSERT_EQ(numCol, 5000);
  ASSERT_EQ(numRow, 5000);
  ASSERT_EQ(numColOffset, 0);
  ASSERT_EQ(numRowOffset, 0);
  ASSERT_EQ(marginX, 0);
  ASSERT_EQ(marginY, 0);
  ASSERT_EQ(numBand, 3);
  ASSERT_EQ(dataType, GDT_Byte);
}

TEST(ImageReader, ReadRaster) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_median.tif"));
  ImageReader imgReader(imgFile);
  imgReader.SetMargin(30, 30, eREFLECTION);

  int32_t numCol, numRow;
  imgReader.GetSize(numCol, numRow);

  int32_t tileSize = 3000;
  for (int i = 0; i < numCol; i += tileSize) {
    for (int j = 0; j < numRow; j += tileSize) {
      int32_t numColRead =
          (i + tileSize > numCol) ? numCol % tileSize : tileSize;
      int32_t numRowRead =
          (j + tileSize > numRow) ? numRow % tileSize : tileSize;
      auto imgWin = imgReader.ReadRaster(i, j, numColRead, numRowRead);

      int32_t numCol, numRow, numColOffset, numRowOffset, marginX, marginY;
      imgWin->GetMargin(marginX, marginY);
      imgWin->GetOffset(numColOffset, numRowOffset);
      imgWin->GetSize(numCol, numRow);

      int16_t numBand = imgWin->GetNumBands();
      GDALDataType dataType = imgWin->GetDataType();

      ASSERT_EQ(numCol, numColRead);
      ASSERT_EQ(numRow, numRowRead);
      ASSERT_EQ(numColOffset, i);
      ASSERT_EQ(numRowOffset, j);
      ASSERT_EQ(marginX, 30);
      ASSERT_EQ(marginY, 30);
      ASSERT_EQ(numBand, 1);
      ASSERT_EQ(dataType, GDT_UInt16);
    }
  }
  ASSERT_EQ(imgReader.GetNumBands(), 1);
  ASSERT_EQ(imgReader.GetDataType(), GDT_UInt16);
}

TEST(ImageWriter, WriteRaster) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_median.tif"));
  ImageReader imgReader(imgFile);

  int32_t numCol, numRow;
  imgReader.GetSize(numCol, numRow);
  std::cout << numCol << " " << numRow << std::endl;

  int16_t nNumBand = imgReader.GetNumBands();

  const std::string imgFileT(TestConfig::OutputPath("image_median_t.tif"));
  ImageWriter imgWriter(imgFileT, numCol, numRow,
                      nNumBand, imgReader.GetDataType());

  OGRSpatialReference ref = imgReader.GetCoordinateSystem();
  imgWriter.SetCoordinateSystem(ref);

  int32_t tile_size = 3000;
  for (int i = 0; i < numCol; i += tile_size) {
    for (int j = 0; j < numRow; j += tile_size) {
      int32_t numColRead =
          (i + tile_size > numCol) ? numCol % tile_size : tile_size;
      int32_t numRowRead =
          (j + tile_size > numRow) ? numRow % tile_size : tile_size;
      std::unique_ptr<ImageWindow> window =
          imgReader.ReadRaster(i, j, numColRead, numRowRead);
      imgWriter.WriteRaster(window);
    }
  }

  ASSERT_EQ(imgWriter.GetNumBands(), 1);
  ASSERT_EQ(imgWriter.GetDataType(), GDT_UInt16);

  // start comparing the output raster and input raster
  auto sourceWindow = imgReader.ReadRaster(0, 0, 33469, 27868);
  auto outputWindow = imgWriter.ReadRaster(0, 0, 33469, 27868);
  auto sourceDataPtr = sourceWindow->GetData();
  auto outputDataPtr = outputWindow->GetData();

  int result = memcmp(sourceDataPtr, outputDataPtr, 2 * 33469 * 27868);
  ASSERT_EQ(result, 0);
}
*/