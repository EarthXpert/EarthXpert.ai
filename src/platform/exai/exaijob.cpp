/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaijob.h"
#include "platform/exai/exaimanager.h"

using earthxpert::ExaiJob;

ExaiJob::ExaiJob() {
  GDALInitialize();
}

int ExaiJob::SetArgs() {
  return 0;
}

int ExaiJob::Run() {
  return 0;
}
