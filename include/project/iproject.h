/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/exproject.h"

#include <memory>

namespace earthxpert {
/**
 *  The interface IProject defines the functionalities to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS IProject {
public:
  enum PrjType {
    ePT_ADS,
    ePT_FRAME,
    ePT_RATIONAL
  };

  virtual ~IProject() = default;

  virtual PrjType GetProjectType() const noexcept = 0;
};

using UPIProject = std::unique_ptr<IProject>;
using SPIProject = std::shared_ptr<IProject>;
}
