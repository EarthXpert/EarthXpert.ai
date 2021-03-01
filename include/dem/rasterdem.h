/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include <string>

namespace earthxpert {
/**
 *
 */
class EXDEM_EXPORTS RasterDem final : public IDem {
public:
  RasterDem(std::string demFile);
  ~RasterDem() = default;

  /**
   *  \brief Fetch the ground coordinate system where the DEM operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  UPSpatialRefSys GetCoordinateSystem() const;

  /**
   *  \brief Fetch the geocoding information of the DEM.
   */
  UPGeocoding GetGeocoding() const;

private:
  std::string mDemFile;

  void Initialize();
};

using UPRasterDem = std::unique_ptr<RasterDem>;
using SPRasterDem = std::shared_ptr<RasterDem>;
}
