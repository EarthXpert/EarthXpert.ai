/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include <string>

namespace earthxpert {
/**
 *
 */
class EXDEM_EXPORTS RasterDem final : public IDem {
public:
  RasterDem(std::string demFile);
  ~RasterDem() = default;

private:
  std::string mDemFile;

  void Initialize();
};

using UPRasterDem = std::unique_ptr<RasterDem>;
using SPRasterDem = std::shared_ptr<RasterDem>;
}
