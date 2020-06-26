/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "platform/gdal/exgdal.h"
#include "platform/utils/icommand.h"
#include "platform/utils/commandinfo.h"

namespace earthxpert {
/**
 *
 */
class EXGDAL_EXPORTS GdalCmd : public ICommand {
public:
  virtual ~GdalCmd() = default;

protected:
};
}
