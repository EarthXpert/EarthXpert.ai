/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "opencv2/imgcodecs.hpp"

TEST(opencv, imgcodecs) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  cv::Mat img = cv::imread(imgFile, cv::IMREAD_LOAD_GDAL);
  ASSERT_TRUE(img.data);
  ASSERT_EQ(img.dims, 2);
  ASSERT_EQ(img.cols, 5000);
  ASSERT_EQ(img.rows, 5000);

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_GDAL.tif"));
  const auto ret = cv::imwrite(imgFileT, img);
  ASSERT_TRUE(ret);
}
