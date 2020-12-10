/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/ortho/orthotask.h"

namespace earthxpert {
class IRasterReader;
class IRasterResample;
class IRasterWriter;

/**
 *  The class OrthoRectifier completes the ortho generation.
 */
class EXORTHO_EXPORTS OrthoRectifier {
public:
  ~OrthoRectifier() = default;

  /**
   *  \brief
   *
   *  \return Returns true if operation succeeds
   */
  bool Process(IRasterReader* raw, IRasterWriter* ortho);

private:
  void Initialize();
};

using UPOrthoRectifier = std::unique_ptr<OrthoRectifier>;
using SPOrthoRectifier = std::shared_ptr<OrthoRectifier>;
} // namespace earthxpert
