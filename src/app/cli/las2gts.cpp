/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/pcutils.h"

using namespace earthxpert;

int main(int argc, char** argv) {
  const std::string lasFile(argv[1]);
  const std::string gtsFile(argv[2]);

  // OpenCV Subdiv2D
  //CvOptions cvOpt;
  //Las2Gts(lasFile, gtsFile, cvOpt);

  // GNU Triangulated Surface
  GtsOptions gtsOpt;
  Las2Gts(lasFile, gtsFile, gtsOpt);
  return 0;
}
