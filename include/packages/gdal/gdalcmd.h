/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/gdal/exgdal.h"
#include "packages/utils/icommand.h"
#include "packages/utils/commandinfo.h"

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
