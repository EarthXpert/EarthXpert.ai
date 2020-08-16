/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/fsutils.h"

#include <sys/types.h>
#include <sys/stat.h>

namespace earthxpert::utils {
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns filename with prefix
 */
std::string 
PathPrefixFilename(const std::string& fileName, const std::string& prefix) {
  fs::path filePath(fileName);
  auto fn = prefix + filePath.filename().string();

  return fn;
}

/**
 *  \brief replace extention for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] ext new extension
 *  \return Returns filename with new extension
 */
std::string
PathReplaceExt(const std::string& fileName, const std::string& ext) {
  fs::path filePath(fileName);
  auto fn = filePath.stem().string() + ext;
  return fn;
}

bool RemoveAll(const fs::path& filePath) {
  if (!fs::exists(filePath)) return true;
  return fs::remove_all(filePath);
}

bool RemoveFile(const fs::path& filePath) {
  if (!fs::exists(filePath)) return true;
  return fs::remove(filePath);
}

/**
 *  \brief Obtain the ID of the physical hard drive where a specified path resides.
 *
 *  \param [in] devPath The path to retrieve.
 *  \todo check device type/class
 */
unsigned int GetDiskId(const std::string& devPath) {
  unsigned int id{};

#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)

  struct _stat s;
  auto ret = _stat(devPath.c_str(), &s);
  if (0 == ret) {
    id = static_cast<unsigned int>(s.st_dev);
  }

#else

  struct stat s;
  auto ret = stat(devPath.c_str(), &s);
  if (-1 != ret) {
    id = static_cast<unsigned int>(minor(s.st_dev) / 16);
  }

#endif

  return id;
}
}
