#include "gtest/gtest.h"
#include "3rd_party/gdal/wxgdal.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  watxtract::GDALInitialize();

  return RUN_ALL_TESTS();
}
