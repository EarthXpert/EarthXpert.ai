/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/trueortho/torthotask.h"

namespace earthxpert {
class IRasterReader;
class IRasterResample;
class IRasterWriter;

/**
 *  The class TorthoRectifier completes the trueortho generation.
 */
class EXTORTHO_EXPORTS TorthoRectifier {
public:
  ~TorthoRectifier() = default;

  /**
   *  \brief
   *
   *  \return Returns true if operation succeeds
   */
  bool Process(IRasterReader* raw, IRasterWriter* ortho);

private:
  void Initialize();
};

using UPTorthoRectifier = std::unique_ptr<TorthoRectifier>;
using SPTorthoRectifier = std::shared_ptr<TorthoRectifier>;
} // namespace earthxpert
