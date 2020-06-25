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

  ~FrameProject() = default;

  PrjType GetProjectType() const noexcept override { return ePT_FRAME; }

  bool Save();     
  bool SaveAs(std::string prjPath);    
  bool Read(std::string prjPath);    

private:
  bool ReadBingo(std::string prjPath);
  bool ReadP4d(std::string prjPath);     
};
}
