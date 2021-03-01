/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "dem/rasterdem.h"
#include <type_traits>

using namespace earthxpert;

/**
 *  \param [in] demFile path to DEM file
 */
RasterDem::RasterDem(std::string demFile) : mDemFile(std::move(demFile)) {
  Initialize();
}

void RasterDem::Initialize() {

}
