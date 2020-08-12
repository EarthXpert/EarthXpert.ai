/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/iprojectmarshaller.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS ProjectMarshaller : public IProjectMarshaller {
public:
  virtual ~ProjectMarshaller() = default;
};
}
