#include "3rd_party/gtest/gtest_main_gdal.h"
#include "testconfig.h"

#include "core/datasource/vectorutils.h"
#include "core/utils/spatiallayout.h"
#include "io/vector/vectorwriter.h"
#include <filesystem>
#include <omp.h>

using namespace earthxpert;
namespace fs = std::filesystem;

TEST(vectorwriter, DUMMY) {
  std::cout << "[   EXAI   ]" << std::endl;
}
