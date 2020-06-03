/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
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
  virtual ~ILayer() {}

  /**
   *  The RasterType defines the raster data structure
   */
  enum class LayerType {
    eUNDEFINED = -1,
    ePOINT,       //
    eLINE,        //
    eMULTILINE,   //
    ePOLYGON,     //
    NumLayerTypes
  };
  using lt = LayerType;

  virtual std::unique_ptr<ILayer> Clone() const = 0;

  /**
   *  \return Returns type of vector data
   */
  virtual LayerType GetLayerType() const noexcept = 0;

  /**
   *  \return Return number of entities
   */
  virtual int GetNumEntities() const noexcept = 0;
};
} // namespace earthxpert
