/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sfm/exsfm.h"

namespace earthxpert {
/**
 *  The interface IFeatureDetect defines the behaviours of feature detection algorithms.
 */
class EXSFM_EXPORTS IFeatureDetect {
public:
  enum FeatureType {
    eHARRIS,
    eORB,
    eSIFT,
    eSURF,
    eSUSAN
  };

  virtual ~IFeatureDetect() = default;
};
}
