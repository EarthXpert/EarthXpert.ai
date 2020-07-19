/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaicmd.h"
#include "3rdparty/gdal/exgdal.h"

using earthxpert::ExaiCmd;

ExaiCmd::ExaiCmd() {
  GDALInitialize();
}
