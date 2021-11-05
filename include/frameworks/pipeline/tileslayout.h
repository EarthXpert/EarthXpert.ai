/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "frameworks/exframeworks.h"

#include <memory>

namespace earthxpert {
/**
 *  The class TilesLayout divides dataset into tiles within a resource limit.
 */
class EXFRAMEWORKS_EXPORTS TilesLayout {
public:
  ~TilesLayout() = default;

private:
  void Initialize();
};

using UPTilesLayout = std::unique_ptr<TilesLayout>;
using SPTilesLayout = std::shared_ptr<TilesLayout>;
} // namespace earthxpert
