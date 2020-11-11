/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/support/supportmarshaller.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS AdsSupportMarshaller : public SupportMarshaller {
public:
  virtual ~AdsSupportMarshaller() = default;
};

using UPAdsSupportMarshaller = std::unique_ptr<AdsSupportMarshaller>;
using SPAdsSupportMarshaller = std::shared_ptr<AdsSupportMarshaller>;
}
