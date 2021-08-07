/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include "io/raster/irasterreader.h"
#include <string>

namespace earthxpert {
/**
 *  The class RasterDem obtains elevations from a digital elevation model
 *  in raster format.
 */
class EXDEM_EXPORTS RasterDem final : public IDem {
public:
  RasterDem(std::string demFile, int band = 1);
  ~RasterDem() = default;

  /**
   *  \return Returns pointer to the raster reader.
   */
  IRasterReader* GetDemReader() const {
    return mDemReader.get();
  }

  /**
   *  \brief set the SRS of the interface
   */
  //void SetSrsIF();

  float GetElev(const Point2d& xy) const override;
  float GetElev(double x, double y) const override {
    return GetElev(Point2d(x, y));
  }

  /**
   *  \brief Fetch the ground coordinate system where the DEM operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  UPSpatialRefSys GetCoordinateSystem() const override;

  /**
   *  \brief Fetch the geocoding information of the DEM.
   */
  UPGeocoding GetGeocoding() const;

private:
  std::string mDemFile;
  UPIRasterReader mDemReader;

  void Initialize();

  bool MapToDem(const Point2d& map, Point2d& dem);
  bool DemToMap(const Point2d& dem, Point2d& map);
};

using UPRasterDem = std::unique_ptr<RasterDem>;
using SPRasterDem = std::shared_ptr<RasterDem>;
}
