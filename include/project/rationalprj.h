/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/project.h"

namespace earthxpert {
/**
 *  The base class RationalProject implementes the behaviour to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS RationalProject final : public Project {
public:
  enum {
    eProjectType = ePT_RATIONAL
  };

  ~RationalProject() = default;

  PrjType GetProjectType() const noexcept override { return ePT_RATIONAL; }

private: 
  friend class RatPrjMarshaller;
};

using UPRationalProject = std::unique_ptr<RationalProject>;
using SPRationalProject = std::shared_ptr<RationalProject>;
}
