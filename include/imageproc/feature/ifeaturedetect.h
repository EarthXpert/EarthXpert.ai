/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "imageproc/eximageproc.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IFeatureDetect defines the behaviours of feature detection algorithms.
 */
class EXIMAGEPROC_EXPORTS IFeatureDetect {
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

using UPIFeatureDetect = std::unique_ptr<IFeatureDetect>;
using SPIFeatureDetect = std::shared_ptr<IFeatureDetect>;
}
