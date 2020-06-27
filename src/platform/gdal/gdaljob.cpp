/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/gdal/gdaljob.h"
#include "platform/gdal/gdalmanager.h"

using namespace earthxpert;
using earthxpert::GdalJob;

GdalJob::GdalJob(std::ostream& out) : mOut(out) {
  GDALInitialize();
}

int GdalJob::SetArgs() {
  return 0;
}

int GdalJob::Run() {
  return 0;
}

void GdalJob::Help() {
  mOut << "Usage: exgdal <command> <command options>" << std::endl;
}
