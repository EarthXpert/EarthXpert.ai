/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "platform/gdal/exgdal.h"
#include "platform/gdal/gdalinfo.h"

namespace earthxpert {
int EXGDAL_EXPORTS GdalInfo(int, char**);
int EXGDAL_EXPORTS GdalTranslate(int, char**);
int EXGDAL_EXPORTS GdalSrsInfo(int, char**);
int EXGDAL_EXPORTS OgrInfo(int, char**);
}
