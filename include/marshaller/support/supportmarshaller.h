/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/support/isupportmarshaller.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS SupportMarshaller : public ISupportMarshaller {
public:
  virtual ~SupportMarshaller() = default;
};
}
