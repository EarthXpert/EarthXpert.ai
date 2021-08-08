/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "dem/tindem.h"
#include "io/vector/vectorreader.h"

#include <type_traits>

using namespace earthxpert;

/**
 *  \param [in] demFile path to DEM file
 */
TinDem::TinDem(std::string demFile, const int layer)
  : mDemFile(std::move(demFile)) {
  mDemReader = std::make_unique<VectorReader>(demFile, std::vector<int>{layer});
  Initialize();
}

void TinDem::Initialize() {

}

float TinDem::GetElev(const Point2d& xy) const {
  return 0;
}

/**
 *  \brief Fetch the ground coordinate system where the DEM operates.
 *
 *  \return Returns pointer to a coordinate system
 */
UPSpatialRefSys TinDem::GetCoordinateSystem() const {
  return {};
}
