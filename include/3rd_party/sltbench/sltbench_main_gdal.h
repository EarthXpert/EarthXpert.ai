#include "sltbench/Bench.h"
#include "3rd_party/gdal/wxgdal.h"

int main(int argc, char **argv) {
  watxtract::GDALInitialize();

  return sltbench::Main(argc, argv);
}
