/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/project.h"

namespace earthxpert {
/**
 *  The base class FrameProject implementes the behaviour to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS FrameProject final : public Project {
public:
  enum {
    ProjectType = ePT_FRAME
  };
  enum ProjectFormat {
    ePF_BINGO,    // Bingo project format: 
    ePF_EPJ,      // EXAI project format: .epj
    ePF_P4D       // Pix4D project format: .p4d
  };

  FrameProject(const FrameProject&) = delete;
  FrameProject(FrameProject&&) = delete;
  ~FrameProject() = default;

  FrameProject& operator=(const FrameProject&) = delete;
  FrameProject& operator=(FrameProject&&) = delete;

  PrjType GetProjectType() const noexcept override { return ePT_FRAME; }

private:
  friend class P4dPrjMarshaller;
};

using UPFrameProject = std::unique_ptr<FrameProject>;
using SPFrameProject = std::shared_ptr<FrameProject>;
}
