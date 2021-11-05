/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/filter/iimagefilter.h"

namespace earthxpert {
/**
 *  The base calss ImageFilter defines the behaviours of template methods.
 */
class EXIMAGEPROC_EXPORTS ImageFilter : public IImageFilter {
public:
  virtual ~ImageFilter() = default;
};
}
