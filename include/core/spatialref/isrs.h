/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"

#include <memory>

namespace earthxpert {
/**
 *  The class ISRS provides service of spatial coordinate system
 */
class EXCORE_EXPORTS ISRS
{
public:
  virtual ~ISRS() = default;

  virtual void GetDatum() = 0;
};

using UPISRS = std::unique_ptr<ISRS>;
using SPISRS = std::shared_ptr<ISRS>;
} // namespace earthxpert
