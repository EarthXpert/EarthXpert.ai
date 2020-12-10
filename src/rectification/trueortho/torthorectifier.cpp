/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/trueortho/torthorectifier.h"
#include "core/utils/irasterresample.h"
#include "frameworks/pipeline/taskschedular.h"
#include "frameworks/pipeline/tileslayout.h"
#include "io/raster/irasterreader.h"
#include "io/raster/rasterwriter.h"

using namespace earthxpert;

void TorthoRectifier::Initialize() {

}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool TorthoRectifier::Process(IRasterReader* const raw, IRasterWriter* const ortho) {
  return true;
}
