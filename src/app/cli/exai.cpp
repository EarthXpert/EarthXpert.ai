/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaijob.h"
#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {
  earthxpert::ExaiJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
