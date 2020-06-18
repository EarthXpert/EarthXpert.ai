/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/pdal/pdaljob.h"

int main(int argc, char* argv[]) {
  earthxpert::thirdparty::PdalJob job;

  pdal::StringList cmdArgs;
  for (int i = 1; i < argc; ++i)
    cmdArgs.push_back(argv[i]);

  pdal::LogPtr logger{new pdal::Log("PDAL", "stderr")};
  return job.execute(cmdArgs, logger);
}
