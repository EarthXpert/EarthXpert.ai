/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "platform/gdal/gdaljob.h"

namespace earthxpert {
int EXGDAL_EXPORTS GdalInfo(int, char**);
int EXGDAL_EXPORTS GdalTranslate(int, char**);
}
