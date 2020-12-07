#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/proj/exproj.h"

#include <iostream>

using namespace earthxpert;

TEST(proj, SRS_PJ) {
  std::string utmWkt{ R"wkt(+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs)wkt" };
  UPPJ pj{ proj_create(PJ_DEFAULT_CTX, utmWkt.c_str()) };
  ASSERT_TRUE(pj) << "create PJ from PROJ.4 WKT";
}

TEST(proj, CT_PJ) {
  std::cout << "[   EXAI   ]" << std::endl;
}

TEST(proj, hgridshift) {
  std::cout << "[   EXAI   ]" << std::endl;
}

TEST(proj, vgridshift) {
  std::cout << "[   EXAI   ]" << std::endl;
}
