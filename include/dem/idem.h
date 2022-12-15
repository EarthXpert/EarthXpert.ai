/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/exdem.h"
#include "core/spatialref/geocoding.h"
#include "core/spatialref/spatialdef.h"

#include <memory>

namespace earthxpert {
/**
 *  \brief the interface IDem defines the behaviours of Digital Elevation Model.
 */
class EXDEM_EXPORTS IDem {
public:
  virtual ~IDem() = default;

  virtual float GetElev(const Point2d& xy) const = 0;
  virtual float operator()(const Point2d& xy) const = 0;

  virtual float GetElev(double x, double y) const = 0;
  virtual float operator()(double x, double y) const = 0;

  /**
   *  \brief Fetch the ground coordinate system where the DEM operates.
   *
   *  \return Returns pointer to a coordinate system
   */
  virtual UPSpatialRefSys GetCoordinateSystem() const = 0;

  //virtual void SetTargetCoordinateSystem() = 0;
};

using UPIDem = std::unique_ptr<IDem>;
using SPIDem = std::shared_ptr<IDem>;
}
