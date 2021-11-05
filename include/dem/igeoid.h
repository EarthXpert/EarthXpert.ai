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
 *  \brief the interface IGeoid defines the behaviours of Geoid.
 */
class EXDEM_EXPORTS IGeoid {
public:
  virtual ~IGeoid() = default;
};

using UPIGeoid = std::unique_ptr<IGeoid>;
using SPIGeoid = std::shared_ptr<IGeoid>;
}
