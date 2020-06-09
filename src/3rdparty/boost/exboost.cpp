/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/boost/exboost.h"
#include "boost/filesystem.hpp"

namespace bfs = boost::filesystem;

namespace earthxpert::thirdparty {
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns file path with prefix
 */
std::string PathPrefixFilename(
  const std::string& fileName, const std::string& prefix) {
  bfs::path filePath(fileName);
  auto fn = prefix + filePath.filename().string();

  return fn;
}
}
