/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/epipolartask.h"

namespace earthxpert {
class IRasterReader;
class IRasterResample;
class IRasterWriter;
class IEpipolarModel;

/**
 *  The class EpipolarRectifier completes the epipolar generation.
 */
class EXEPIPOLAR_EXPORTS EpipolarRectifier {
public:
  ~EpipolarRectifier() = default;

  /**
   *  \brief
   *
   *  \return Returns true if operation succeeds
   */
  bool Process(IRasterReader* img, IRasterWriter* epi, IEpipolarModel const* epiModel);

private:
  void Initialize();
};

using UPEpipolarRectifier = std::unique_ptr<EpipolarRectifier>;
using SPEpipolarRectifier = std::shared_ptr<EpipolarRectifier>;
} // namespace earthxpert
