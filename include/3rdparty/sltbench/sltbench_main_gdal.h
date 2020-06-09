#include "sltbench/Bench.h"
#include "3rdparty/gdal/exgdal.h"

int main(int argc, char **argv) {
  earthxpert::GDALInitialize();

  return sltbench::Main(argc, argv);
}
