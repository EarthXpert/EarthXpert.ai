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

/**
 *  \brief Fetch the ground coordinate system where the DEM operates.
 *
 *  \return Returns pointer to a coordinate system
 */
UPSpatialRefSys RasterDem::GetCoordinateSystem() const {
  return {};
}

/**
 *  \brief Fetch the geocoding information of the DEM.
 */
UPGeocoding RasterDem::GetGeocoding() const {
  return {};
}
