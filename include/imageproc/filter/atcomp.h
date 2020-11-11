/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/filter/imagefilter.h"

namespace earthxpert {
/**
 *  The class AtComp improve image statistically and virtually by
 *  compensating the atmospheric effects, such as haze, blur, shadow etc.
 */
class EXIMAGEPROC_EXPORTS AtComp final : public ImageFilter {
public:
  ~AtComp() = default;

  bool Process(const IRaster* src, IRaster* tgt) override;
};
}
