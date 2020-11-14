/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "gtest/gtest.h"
#include "3rdparty/gdal/exgdal.h"
#include <vld.h>

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  earthxpert::GDALInitialize();

  return RUN_ALL_TESTS();
}
