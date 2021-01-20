/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/bbaimageset.h"
#include "blockadjust/rationalbbaimage.h"

#include <vector>

namespace earthxpert {
/**
 *  The class RationalBbaImageSet defines the behaviours of image bba info.
 */
class EXBBA_EXPORTS RationalBbaImageSet /*final : public BbaImageSet*/ {
public:
  RationalBbaImageSet(const RationalBbaImageSet&) = delete;
  RationalBbaImageSet(RationalBbaImageSet&&) = delete;
  ~RationalBbaImageSet() = default;

  RationalBbaImageSet& operator=(const RationalBbaImageSet&) = delete;
  RationalBbaImageSet& operator=(RationalBbaImageSet&&) = delete;

private:
  friend class RationalCeresSolver;

  std::vector<std::unique_ptr<RationalBbaImage>> mBbaImages;
};

using UPRationalBbaImageSet = std::unique_ptr<RationalBbaImageSet>;
using SPRationalBbaImageSet = std::shared_ptr<RationalBbaImageSet>;
} // namespace earthxpert
