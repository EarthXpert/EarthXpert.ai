/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/hana/core.hpp>
#include <boost/hana/type.hpp>

namespace bhn = boost::hana;

TEST(boost, hana) {
  ASSERT_TRUE(bhn::true_::value);
}
