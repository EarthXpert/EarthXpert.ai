/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/ortho/orthotask.h"
#include "rectification/ortho/orthorect.h"

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

  //std::unique_ptr<uchar[]> mImgMaskBuf;   // image mask
  //std::unique_ptr<uchar[]> mOrthoMaskBuf;   // ortho mask
};

using UPOrthoRectifier = std::unique_ptr<OrthoRectifier>;
using SPOrthoRectifier = std::shared_ptr<OrthoRectifier>;
} // namespace earthxpert
