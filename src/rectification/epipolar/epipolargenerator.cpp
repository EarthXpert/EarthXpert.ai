/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolargenerator.h"
#include "core/utils/irasterresample.h"
#include "epipolarmodel/iepipolarmodel.h"
#include "io/raster/rasterreader.h"
#include "io/raster/rasterwriter.h"

using earthxpert::EpipolarGenerator;
using earthxpert::IEpipolarModel;
using earthxpert::IRasterReader;
using earthxpert::IRasterResample;
using earthxpert::IRasterWriter;

/**
 *  \brief
 */
EpipolarGenerator::EpipolarGenerator(
    const IRasterResample* rasResample, const size_t memSize)
    : mRasResample(rasResample), mMemSize(memSize) {
}

/**
 *  \brief
 *
 *  \return Returns true if operation succeeds
 */
bool EpipolarGenerator::Process(IRasterReader *const img,
                                IRasterWriter *const epi,
                                IEpipolarModel const *const epiModel,
                                const IEpipolarModel::StereoPos pos) {
  return true;
}
