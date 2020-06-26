/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/gdal/gdaljob.h"
#include "platform/gdal/gdalmanager.h"

using namespace earthxpert;
using earthxpert::GdalJob;

GdalJob::GdalJob() {
}

int GdalJob::SetArgs() {
  return 0;
}

int GdalJob::Run() {
  GdalInfo(0, nullptr);
  GdalTranslate(0, nullptr);
  return 0;
}
