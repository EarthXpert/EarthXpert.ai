#include "gtest/gtest.h"
#include "3rdparty/gdal/exgdal.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  earthxpert::GDALInitialize();

  return RUN_ALL_TESTS();
}
