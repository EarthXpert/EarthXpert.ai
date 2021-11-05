/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <memory>

namespace earthxpert {
/**
 * The interface IAlgorithm defines the behaviour of algorithms.
 */
class EXCORE_EXPORTS IAlgorithm {
public:
  virtual ~IAlgorithm() = default;
};

using UPIAlgorithm = std::unique_ptr<IAlgorithm>;
using SPIAlgorithm = std::shared_ptr<IAlgorithm>;
}
