/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/filter/imagefilter.h"

namespace earthxpert {
/**
 *  The calss Dehaze improve image statistically and virtually by 
 *  eliminating image hae, blur, shadow etc.
 */
class EXIMAGEPROC_EXPORTS Dehaze final : public ImageFilter {
public:
  ~Dehaze() = default;

  bool Process(const IRaster* src, IRaster* tgt) override;
};
}
