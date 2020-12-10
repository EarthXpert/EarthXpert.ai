/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/ortho/orthorectifier.h"
#include "core/utils/irasterresample.h"
#include "frameworks/pipeline/taskschedular.h"
#include "frameworks/pipeline/tileslayout.h"
#include "io/raster/irasterreader.h"
#include "io/raster/rasterwriter.h"

using namespace earthxpert;

void OrthoRectifier::Initialize() {

}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool OrthoRectifier::Process(IRasterReader* const raw, IRasterWriter* const ortho) {
  return true;
}
