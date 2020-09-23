/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace earthxpert::utils {
enum CombineType {
  eALL,     // all components
  eCOMMON,  // common 
  eDIFF     // different 
};

/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns prefix + filename
 */
std::string EXCORE_EXPORTS PathPrefixFilename(
  const std::string& fileName, const std::string& prefix);
std::string EXCORE_EXPORTS PathSuffixFilename(
  const std::string& fileName, const std::string& suffix);
std::string EXCORE_EXPORTS PathReplaceExt(
  const std::string& fileName, const std::string& ext);

bool EXCORE_EXPORTS RemoveAll(const fs::path& filePath);
inline bool RemoveAll(const std::string& filePath) {
  return RemoveAll(fs::path(filePath));
}

bool EXCORE_EXPORTS RemoveFile(const fs::path& filePath);
inline bool RemoveFile(const std::string& filePath) {
  return RemoveFile(fs::path(filePath));
}

std::string EXCORE_EXPORTS CombineFilenames(const std::string& s1,
  const std::string& s2, const std::string& delim = "_-", CombineType combType= eALL);

unsigned int EXCORE_EXPORTS GetDiskId(const std::string& devPath);
} // namespace earthxpert::utils
