/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/exai/exaicmd.h"
#include "3rdparty/gdal/exgdal.h"

using earthxpert::ExaiCmd;

ExaiCmd::ExaiCmd() {
  GDALInitialize();
}

//int ExaiCmd::Execute() {
//  return {};
//}
