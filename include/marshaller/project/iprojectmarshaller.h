/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

#include <memory>

namespace earthxpert {
class EXMARSHALLER_EXPORTS IProjectMarshaller {
public:
  virtual ~IProjectMarshaller() = default;
};

using UPIProjectMarshaller = std::unique_ptr<IProjectMarshaller>;
using SPIProjectMarshaller = std::shared_ptr<IProjectMarshaller>;
}
