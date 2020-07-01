/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/gdal/gdaljob.h"
#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {
  earthxpert::GdalJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
