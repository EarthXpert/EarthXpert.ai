/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolarrectifier.h"
#include "core/utils/irasterresample.h"
#include "core/utils/geomutils.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "frameworks/pipeline/taskschedular.h"
#include "frameworks/pipeline/tileslayout.h"
#include "io/raster/irasterreader.h"
#include "io/raster/rasterwriter.h"

using namespace earthxpert;

void EpipolarRectifier::Initialize() {

}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool EpipolarRectifier::Process(IRasterReader* const img,
  IRasterWriter* const epi, IEpipolarModel const* epiModel) {
  return true;
}
