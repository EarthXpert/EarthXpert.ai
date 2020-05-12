/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "srcconfig.h"
#include "core/utils/rasterlayout.h"

#include <iostream>

using watxtract::RasterLayout;
using watxtract::RasterWindow;

int main(int argc, char** argv) {
  // get full path of command line app by name
  std::string exe_path = SdkConfig::BinPath("wxgeoinfo");
  std::cerr << "Full path: " << exe_path << std::endl;

  // call WXAI's APIs
  RasterLayout rasterLayout(1000, 1000, 600, 600);
  auto& rasterWins = rasterLayout.GetRasterWindows();

  int i{};
  for (auto& rw : rasterWins) {
    std::cerr << "RasterWindow #" << i++ << ": " << rw.colOffset << ',' << rw.rowOffset << ',' << rw.numCols << ',' << rw.numRows << std::endl;
  }

  return 0;
}
