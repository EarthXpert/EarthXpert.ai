/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/ogrlayer.h"

using namespace earthxpert;

OgrLayer::OgrLayer(OGRLayer* layer) : mLayer(layer) {
  ObtainLayerInfor();
}

void OgrLayer::ObtainLayerInfor() {
}
