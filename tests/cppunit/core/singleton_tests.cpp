/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/patterns/singleton.h"

using namespace earthxpert;

TEST(singleton, instance) {
  auto& inst1 = Singleton::Instance();
  auto& inst2 = Singleton::Instance();
  
  ASSERT_EQ(&inst1, &inst2);
}
