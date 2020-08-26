/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/eximageproc.h"
#include "core/datasource/iraster.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IImageFilter defines the behaviours of image processing algorithms.
 */
class EXIMAGEPROC_EXPORTS IImageFilter {
public:
  virtual ~IImageFilter() = default;

  virtual bool Process(const IRaster* src, IRaster* tgt) = 0;
};

using UPIImageFilter = std::unique_ptr<IImageFilter>;
using SPIImageFilter = std::shared_ptr<IImageFilter>;
}
