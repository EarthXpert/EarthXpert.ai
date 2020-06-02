/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

namespace earthxpert {
/**
 *  The structure RegionTile contains the offset and size information.
 */
template <typename Ty>
struct EXCORE_EXPORTS RegionTile {
  Ty xmin{};
  Ty ymin{};
  Ty width{};
  Ty height{};

  void SetOffsets(const Ty xmin, const Ty ymin) {
    this->xmin = xmin;
    this->ymin = ymin;
  }

  void SetSize(const Ty width, const Ty height) {
    this->width  = width;
    this->height = height;
  }

  void SetMargins(const Ty marginWidth, const Ty marginHeight) {
    xmin -= marginWidth;
    ymin -= marginHeight;
    width  += marginWidth * 2;
    height += marginHeight * 2;
  }
};
}
