/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <memory>

namespace earthxpert {
/**
 *  The class ISrs provides service of spatial coordinate system
 */
class EXCORE_EXPORTS ISrs
{
public:
  virtual ~ISrs() = default;

  virtual void GetDatum() = 0;
};

using UPISrs = std::unique_ptr<ISrs>;
using SPISrs = std::shared_ptr<ISrs>;
} // namespace earthxpert
