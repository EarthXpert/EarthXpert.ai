/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "sensormodel/frameep.h"
#include "sensormodel/frameip.h"
#include "sensormodel/framemodel.h"

#include <cmath>
#include <iostream>

using namespace earthxpert;

static std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel();

/**
 * Test issue: segmentation fault.
 */
TEST(framemodel, CreateFrameIP) {
  std::string camName{"CameraName"};
  double focalLength{1.0};
  std::array<double, 2> ppa{0.0, 0.0};
  std::array<double, 6> radialDist{0.0};
  std::array<double, 4> decentDist{0.0};

  auto frameIP = std::make_unique<FrameIP>(camName, focalLength, ppa,
                                           radialDist, decentDist);

  ASSERT_TRUE(nullptr != frameIP) << "returned nullptr";
}

TEST(framemodel, CreateFrameEP) {
  std::cerr << "[   WXAI   ] "
            << "not implemented" << std::endl;
}

TEST(framemodel, CreateDigitalFrameModel) {
  const auto rationalModel = CreateDigitalFrameModel();

  ASSERT_TRUE(nullptr == rationalModel) << "returned nullptr";
}

/**
 * Create an instance of digital frame model
 */
std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel() {
  return nullptr;
}
