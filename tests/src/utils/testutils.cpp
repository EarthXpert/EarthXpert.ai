/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "testutils.h"

#include <boost/format.hpp>
#include <fstream>

namespace earthxpert::testutils {
/**
 *
 */
void SaveCurves(const std::vector<Point3d>& curves,
  const int numLines, const std::string& txtFile) {
  std::ofstream ofs(txtFile.c_str());
  ofs << boost::format("%1% %2% %3%") %
    curves.size() % numLines % (curves.size()/numLines);

  for (auto& grd : curves) {
    ofs << boost::format("\n%.6f %.6f %.6f") % grd.x % grd.y % grd.z;
  }
}
}
