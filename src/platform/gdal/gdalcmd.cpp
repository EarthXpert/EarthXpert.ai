/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaijob.h"
#include "platform/exai/exaimanager.h"

using earthxpert::ExaiJob;

ExaiJob::ExaiJob(std::ostream& out) : mOut(out) {
  GDALInitialize();
}

int ExaiJob::Run() {
  return 0;
}

void ExaiJob::Help() {
  mOut << "Usage: exai <command> <command options>" << std::endl;
}
