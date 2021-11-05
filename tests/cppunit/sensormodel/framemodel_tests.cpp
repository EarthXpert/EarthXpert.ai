/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "sensormodel/frameep.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/utils/frameip.h"

#include <cmath>
#include <iostream>

using namespace earthxpert;

std::unique_ptr<FrameIP> CreateFrameIP();
std::unique_ptr<FrameEP> CreateFrameEP();
std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel();

std::unique_ptr<FilmFrameModel> CreateFilmFrameModel();

/**
 * Test issue: segmentation fault.
 */
TEST(framemodel, CreateFrameIP) {
  auto frameIP = CreateFrameIP();
  ASSERT_TRUE(nullptr != frameIP) << "returned nullptr";
}

TEST(framemodel, CreateFrameEP) {
  auto frameEP = CreateFrameEP();
  ASSERT_TRUE(nullptr != frameEP) << "returned nullptr";
}

TEST(framemodel, CreateDigitalFrameModel) {
  const auto rationalModel = CreateDigitalFrameModel();

  ASSERT_TRUE(nullptr == rationalModel) << "returned nullptr";
}

/**
 * Create an instance of digital frame model
 */
std::unique_ptr<FrameIP> CreateFrameIP() {
  std::string camName{ "CameraName" };
  double focalLength{ 1.0 };
  std::array<double, 2> ppa{ 0.0, 0.0 };
  std::array<double, FrameIP::NumRadCoef> radialDist{ 0.0 };
  std::array<double, FrameIP::NumDecCoef> decentDist{ 0.0 };

  return std::make_unique<FrameIP>(camName, 
    focalLength, ppa, radialDist, decentDist);
}

std::unique_ptr<FrameEP> CreateFrameEP() {
  std::array<double, 3> position{};
  std::array<double, 3> rotation{};
  SPSpatialRefSys groundSRS;

  return std::make_unique<FrameEP>(position, rotation, groundSRS);
}

std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel() {
  DigitalImageModel imgModel(std::make_shared<DigitalImageModelImp>(1, 1, 1, 1));
  auto frameIP = CreateFrameIP();
  auto frameEP = CreateFrameEP();

  return std::make_unique<DigitalFrameModel>(std::move(imgModel), 
    std::move(frameIP), std::move(frameEP));
}


std::unique_ptr<FilmFrameModel> CreateFilmFrameModel() {
  return nullptr;
}
