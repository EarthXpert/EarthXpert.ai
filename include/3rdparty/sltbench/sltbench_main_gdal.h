/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sltbench/Bench.h"
#include "3rdparty/gdal/exgdal.h"
#include <vld.h>

int main(int argc, char **argv) {
  earthxpert::GDALInitialize();

  return sltbench::Main(argc, argv);
}
