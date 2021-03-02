/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "dem/rasterdem.h"
#include "io/raster/rasterreader.h"

#include <type_traits>

using namespace earthxpert;

/**
 *  \param [in] demFile path to DEM file
 */
RasterDem::RasterDem(std::string demFile, const int band) 
  : mDemFile(std::move(demFile)) {
  mDemReader = std::make_unique<RasterReader>(demFile, std::vector<int>{band});
  Initialize();
}

void RasterDem::Initialize() {

}

float RasterDem::GetElev(const double x, const double y) const {
  return 0;
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

/**
 *  \brief convert ground position in map to pixel position in DEM
 */
bool RasterDem::MapToDem(const Point2d& map, Point2d& dem) {
  return true;
}

/**
 *  \brief convert pixel position in DEM to ground position in map
 */
bool RasterDem::DemToMap(const Point2d& dem, Point2d& map) {
  return true;
}
