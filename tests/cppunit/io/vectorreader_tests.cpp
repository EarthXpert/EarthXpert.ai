#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/datasource/vectorutils.h"
#include "core/utils/regionlayout.h"
#include "io/vector/vectorreader.h"
#include <filesystem>
#include <omp.h>

using namespace earthxpert;
namespace fs = std::filesystem;

TEST(vectorreader, DUMMY) {
  std::cout << "[   EXAI   ]" << std::endl;
}
