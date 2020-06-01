#include "3rd_party/gtest/gtest_main.h"
#include "testconfig.h"

#include "3rd_party/laslib/exlaslib.h"
#include <filesystem>

namespace fs = std::filesystem;

TEST(laslib, CreateLAX) {
  const std::string lasFile(TestConfig::DataPath("PointClouds/points_small.las"));
  const auto laxFile = earthxpert::thirdparty::CreateLAX(lasFile);

  ASSERT_TRUE(fs::exists(laxFile));
}
