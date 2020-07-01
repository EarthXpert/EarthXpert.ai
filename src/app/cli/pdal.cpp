/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/pdal/pdaljob.h"
#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {
  earthxpert::PdalJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
