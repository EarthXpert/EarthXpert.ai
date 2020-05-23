#include "sltbench/Bench.h"
#include "3rd_party/gdal/exgdal.h"

int main(int argc, char **argv) {
  earthxpert::GDALInitialize();

  return sltbench::Main(argc, argv);
}
