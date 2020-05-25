/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/densematch/idensematch.h"

namespace earthxpert {

/**
 *  The class DenseMatchFactory defines the behaviours for creating
 *  dense matcher objects.
 */
class EXSFM_EXPORTS DenseMatchFactory {
public:
  /**
   *  \brief
   */
  static std::unique_ptr<IDenseMatch> CreateDenseMatch();

  /**
   *  \brief
   */
  static std::unique_ptr<IDenseMatch> CreateDepthMatch();

  /**
   *  \brief
   */
  static std::unique_ptr<IDenseMatch> CreateDisparityMatch();
};
} // namespace earthxpert
