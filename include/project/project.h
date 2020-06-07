/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/iproject.h"

namespace earthxpert {
/**
 *  The base class Project implementes teh template methods to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS Project : public IProject {
public:
  virtual ~Project() = default;
};
}
