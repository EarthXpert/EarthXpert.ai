/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/boost/exboost.h"

//#include "algo.hpp"
#include "fusion.hpp"
#include "datetime.hpp"
#include "geometry.hpp"
#include "graph.hpp"
#include "hana.hpp"
#include "math.hpp"
#include "mp11.hpp"
#include "mpl.hpp"
#include "multiarray.hpp"
//#include "multiprecision.hpp"
#include "numpy.hpp"
#include "python.hpp"
#include "spirit.hpp"
#include "timer.hpp"

#include <boost/pool/object_pool.hpp>

TEST(bfs, path) {
  bfs::path filePath("D:/Workspace/Results/output.txt");

  ASSERT_FALSE(filePath.filename_is_dot());
  ASSERT_TRUE(filePath.has_extension());
}
