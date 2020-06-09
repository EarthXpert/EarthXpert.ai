/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"
#include <string>

namespace earthxpert::thirdparty {
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns prefix + filename
 */
std::string EX3RDPARTY_EXPORTS PathPrefixFilename(
  const std::string& fileName, const std::string& prefix);
} // namespace earthxpert::thirdparty
