/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rd_party/gdal/exgdal.h"
#include "core/datasource/vectortypes.h"

#include <memory>
#include <utility>

namespace earthxpert {
/**
 *  The interface ILayer defines a vector layer, that provides
 *  information about vector type, graphical entities.
 */
class EXCORE_EXPORTS ILayer {
public:
  virtual ~ILayer() = default;

  virtual std::unique_ptr<ILayer> Clone() const = 0;

  /**
   *  \return Returns vector layer information
   */
  virtual LayerInfo GetLayerInfo() const noexcept = 0;

  /**
   *  \return Return number of entities
   */
  virtual int GetNumFeatures() const noexcept = 0;
};
} // namespace earthxpert
