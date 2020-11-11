/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/exai/exaijob.h"
#include "packages/exai/exaimanager.h"

using earthxpert::ExaiJob;

ExaiJob::ExaiJob(std::ostream& out) : mOut(out) {
}

/**
 *  \brief
 */
int ExaiJob::Run() {
  return 0;
}

void ExaiJob::Help() {
  mOut << "Usage: exai <command> <command options>" << std::endl;
}
