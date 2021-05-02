/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/typehelper.h"

using namespace earthxpert;

TEST(typehelper, Overloaded) {
    const auto ol = Overloaded{
        [](const int i) { return i * 1; },
        [](const float f) { return f * 2; },
        [](const double d) { return d * 3; },
    };

    ASSERT_EQ(10, ol(10));
    ASSERT_EQ(20, ol(10.0f));
    ASSERT_EQ(30, ol(10.0));
}
