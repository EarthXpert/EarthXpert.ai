/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/blockadjust.h"
#include "blockadjust/rationalbbaimageset.h"

namespace earthxpert {
/**
 *  The class RationalBBA defines the behaviours of adjusting
 *  rational function model based universal imaging systems.
 */
class EXBBA_EXPORTS RationalBBA final : public BlockAdjust {
public:
  RationalBBA(const RationalBBA&) = delete;
  RationalBBA(RationalBBA&&) = delete;
  ~RationalBBA() = default;

  RationalBBA& operator=(const RationalBBA&) = delete;
  RationalBBA& operator=(RationalBBA&&) = delete;

private:
  std::vector<std::unique_ptr<RationalBbaImageSet>> mImgSet;
};

using UPRationalBBA = std::unique_ptr<RationalBBA>;
using SPRationalBBA = std::shared_ptr<RationalBBA>;
} // namespace earthxpert
