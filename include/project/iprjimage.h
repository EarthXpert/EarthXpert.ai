/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/exproject.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IPrjImage defines the functionalities to access entires
 *  of an imagery data.
 */
class EXPROJECT_EXPORTS IPrjImage {
public:
  virtual ~IPrjImage() = default;

  //virtual ImageType GetImageType() = 0;
};

using UPIPrjImage = std::unique_ptr<IPrjImage>;
using SPIPrjImage = std::shared_ptr<IPrjImage>;
}
