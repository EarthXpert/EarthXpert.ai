/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/project.h"

namespace earthxpert {
/**
 *  The base class SarProject implementes the behaviour to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS SarProject final : public Project {
public:
  enum {
    ProjectType = ePT_SAR
  };

  ~SarProject() = default;

  PrjType GetProjectType() const noexcept override { return ePT_SAR; }

private: 
  friend class SarPrjMarshaller;
};

using UPSarProject = std::unique_ptr<SarProject>;
using SPSarProject = std::shared_ptr<SarProject>;
}
