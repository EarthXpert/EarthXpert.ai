/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/ilayer.h"

#include "ogrsf_frmts.h"

namespace earthxpert {
/**
 *  The class OgrLayer defines a vector layer, that provides
 *  information about vector type, graphical entities with OGRLayer as backend.
 */
class EXCORE_EXPORTS OgrLayer final : public ILayer {
public:
  OgrLayer(OGRLayer* layer);
  ~OgrLayer() = default;

  /**
   *  \return Returns the backend OGRLayer object. Ogrlayer
   *  exports partially the OGR functions. The caller could exploit
   *  the full OGR capabilities via the returned object pointer.
   */
  OGRLayer* GetOGRLayer() const noexcept { return mLayer.get(); }

  std::unique_ptr<ILayer> Clone() const override {
    return nullptr;
  }

  /**
   *  \return Returns vector layer information
   */
  LayerInfo GetLayerInfo() const noexcept override { return mLayerInfo; }

  /**
   *  \return Return number of entities
   */
  int GetNumFeatures() const noexcept override { return 0; }

private:
  std::unique_ptr<OGRLayer> mLayer;   // internal OGR backend

  // cache
  LayerInfo mLayerInfo;

  void ObtainLayerInfor();
};
} // namespace earthxpert
