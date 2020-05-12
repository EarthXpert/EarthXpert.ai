/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/wxproject.h"

namespace watxtract {
/**
 *  The interface IProject defines the functionalities to access entires
 *  of a project composed of imagery data.
 */
class WXPROJECT_EXPORTS IProject {
public:
  virtual ~IProject() {}
}
