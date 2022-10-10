/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "testutils.h"
#include "core/exceptions/exexcept.h"

#include <boost/algorithm/string.hpp>
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

std::size_t ReadLsr2UtmCSV(const std::string& csvFile, 
  std::vector<Point3d>& lsrs, std::vector<Point2d>& utms) {
  std::ifstream ifs(csvFile.c_str());
  if (!ifs.good())
    throw std::invalid_argument(_("Failed to open file."));

  std::string line;
  std::getline(ifs, line);  // skip header columns

  while (!ifs.eof()) {
    std::getline(ifs, line);
    boost::trim(line);
    if (line[0] == '#')
      continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" ,\t\n"),
      boost::algorithm::token_compress_on);

    if (6 != tokens.size())
      throw std::exception(_("Each line of grid point must contain 6 fields"));

    const double x = std::stod(tokens[1]);
    const double y = std::stod(tokens[2]);
    const double z = std::stod(tokens[3]);
    lsrs.push_back(Point3d(x, y, z));

    const double easting = std::stod(tokens[4]);
    const double northing = std::stod(tokens[5]);
    utms.push_back(Point2d(easting, northing));
  }

  return lsrs.size();
}
}
