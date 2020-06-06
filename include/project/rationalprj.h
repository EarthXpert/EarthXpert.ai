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
  ~RationalProject() = default;
};
}
