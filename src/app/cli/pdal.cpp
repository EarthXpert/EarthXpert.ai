/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/services/pdaljob.h"

int main(int argc, char* argv[]) {
  earthxpert::PdalJob job;

  pdal::StringList cmdArgs;
  for (int i = 1; i < argc; ++i)
    cmdArgs.push_back(argv[i]);

  job.SetArgs(std::move(cmdArgs));
  return job.Run();
}
