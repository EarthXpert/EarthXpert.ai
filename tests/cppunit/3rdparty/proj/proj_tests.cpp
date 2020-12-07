#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/proj/exproj.h"

#include <iostream>

TEST(proj, crs) {
  std::cout << "[   EXAI   ]" << std::endl;

  std::string utmWkt{ R"wkt(+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs)wkt" };
}

TEST(proj, ct) {
  std::cout << "[   EXAI   ]" << std::endl;
}

TEST(proj, hgridshift) {
  std::cout << "[   EXAI   ]" << std::endl;
}

TEST(proj, vgridshift) {
  std::cout << "[   EXAI   ]" << std::endl;
}
