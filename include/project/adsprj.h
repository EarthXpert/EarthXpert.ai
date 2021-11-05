/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/project.h"

namespace earthxpert {
/**
 *  The base class AdsProject implementes the behaviour to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS AdsProject final : public Project {
public:
  enum {
    ProjectType = ePT_ADS
  };

  ~AdsProject() = default;

  PrjType GetProjectType() const noexcept override { return ePT_ADS; }

private:
  friend class AdsPrjMarshaller;
};

using UPAdsProject = std::unique_ptr<AdsProject>;
using SPAdsProject = std::shared_ptr<AdsProject>;
}
