/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/fsutils.h"

namespace earthxpert::utils {
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns file path with prefix
 */
std::string PathPrefixFilename(
  const std::string& fileName, const std::string& prefix) {
  fs::path filePath(fileName);
  auto fn = prefix + filePath.filename().string();

  return fn;
}
}
