/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main_gdal.h"
#include "testconfig.h"
#include "3rdparty/opencv/exopencv.h"

#include "opencv2/imgcodecs.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

TEST(opencv, info) {
  std::cout << cv::getBuildInformation() << std::endl;

  std::cout << "OpenCV parallel region: " << cv::getNumThreads() << std::endl;

  cv::setNumThreads(6);
  ASSERT_EQ(6, cv::getNumThreads()) << "OpenCV parallel region";
}

TEST(opencv, imgcodecs) {
  const std::string imgFile(TestConfig::DataPath("Imagery/image_small.tif"));
  cv::Mat img = cv::imread(imgFile, cv::IMREAD_LOAD_GDAL);
  ASSERT_TRUE(img.data);
  ASSERT_EQ(img.dims, 2);
  ASSERT_EQ(img.cols, 5000);
  ASSERT_EQ(img.rows, 5000);

  const std::string imgFileT(TestConfig::OutputPath("image_small_t_CVGDAL.tif"));
  const auto ret = cv::imwrite(imgFileT, img);
  ASSERT_TRUE(ret);
  ASSERT_TRUE(fs::exists(fs::path(imgFileT)));
}
