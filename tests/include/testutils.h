/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "extestutils.h"
#include "3rdparty/opencv/exopencv.h"

#include <string>
#include <vector>

namespace earthxpert::testutils {
void EXTESTUTILS_EXPORTS SaveCurves(const std::vector<Point3d>& curves, 
  int numLines, const std::string& txtFile);

std::size_t EXTESTUTILS_EXPORTS ReadLsr2UtmCSV(const std::string& csvFile, 
  std::vector<Point3d>& lsrs, std::vector<Point2d>& utms);
}
