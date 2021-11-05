/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/gdal/gdaljob.h"
#include "packages/gdal/gdalmanager.h"

using namespace earthxpert;
using earthxpert::GdalJob;

GdalJob::GdalJob(std::ostream& out) : mOut(out) {
  GDALInitialize();
}

/**
 *  \brief
 */
int GdalJob::Run() {
  return 0;
}

void GdalJob::Help() {
  mOut << "Usage: exgdal <command> <command options>" << std::endl;
}
