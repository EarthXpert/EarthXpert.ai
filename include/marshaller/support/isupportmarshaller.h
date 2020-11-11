/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

#include <memory>

namespace earthxpert {
class EXMARSHALLER_EXPORTS ISupportMarshaller {
public:
  virtual ~ISupportMarshaller() = default;
};

using UPISupportMarshaller = std::unique_ptr<ISupportMarshaller>;
using SPISupportMarshaller = std::shared_ptr<ISupportMarshaller>;
}
