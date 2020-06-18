/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/pdal/pdalapp.h"

int main(int argc, char* argv[]) {
  earthxpert::thirdparty::PdalApp pa;

  pdal::StringList cmdArgs;
  for (int i = 1; i < argc; ++i)
    cmdArgs.push_back(argv[i]);

  pdal::LogPtr logger{new pdal::Log("PDAL", "stderr")};
  return pa.execute(cmdArgs, logger);
}
