/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/fsutils.h"

#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>

namespace earthxpert {
namespace detail {
std::string CombineTokensAll(const std::vector<std::string>& tokens1, 
  const std::vector<std::string>& tokens2, const char delim='_') {
  const size_t len = tokens1.size();
  assert(len == tokens2.size());

  std::string s;
  for (size_t i = 0; i < len; ++i) {
    if (!s.empty())
      s += delim;
    if (tokens1[i] == tokens2[i]) {
      s += tokens1[i];
    }
    else {
      s += tokens1[i] + delim + tokens2[i];
    }
  }
  return s;
}

std::string CombineTokensCommon(const std::vector<std::string>& tokens1,
  const std::vector<std::string>& tokens2, const char delim = '_') {
  const size_t len = tokens1.size();
  assert(len == tokens2.size());

  std::string s;
  for (size_t i = 0; i < len; ++i) {
    if (!s.empty())
      s += delim;
    if (tokens1[i] == tokens2[i]) {
      s += tokens1[i];
    }
  }
  return s;
}

std::string CombineTokensDiff(const std::vector<std::string>& tokens1,
  const std::vector<std::string>& tokens2, const char delim = '_') {
  const size_t len = tokens1.size();
  assert(len == tokens2.size());

  std::string s;
  for (size_t i = 0; i < len; ++i) {
    if (!s.empty())
      s += delim;
    if (tokens1[i] != tokens2[i]) {
      s += tokens1[i] + delim + tokens2[i];
    }
  }
  return s;
}
}

namespace utils {
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
 *  \brief insert suffix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] suffix string to append
 *  \return Returns filename with suffix
 */
std::string
PathSuffixFilename(const std::string& fileName, const std::string& suffix) {
  fs::path filePath(fileName);
  auto fn = filePath.stem().string() + suffix + filePath.extension().string();
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
 *  \brief combine two filenames having same pattern in naming convention.
 *
 *  \return Returns the combined filename
 */
std::string EXCORE_EXPORTS 
CombineFilenames(const std::string& s1, const std::string& s2, 
  const std::string& delim, const CombineType combType) {
  if (delim.empty())
    return {};

  std::vector<std::string> tokens1;
  boost::split(tokens1, s1, boost::is_any_of(delim),
    boost::algorithm::token_compress_on);
  if (tokens1.empty())
    return s2;

  std::vector<std::string> tokens2;
  boost::split(tokens2, s2, boost::is_any_of(delim),
    boost::algorithm::token_compress_on);
  if (tokens2.empty())
    return s1;

  if (tokens1.size() != tokens2.size())
    return {};

  switch (combType) {
  case eALL:
    return detail::CombineTokensAll(tokens1, tokens2);

  case eCOMMON:
    return detail::CombineTokensCommon(tokens1, tokens2);

  case eDIFF:
    return detail::CombineTokensDiff(tokens1, tokens2);

  default:
    return {};
  }
}

/**
 *  \brief Obtain the ID of the physical hard drive where a specified path resides.
 *
 *  \param [in] devPath The path to retrieve.
 *  \todo check device type/class
 */
unsigned GetDiskId(const std::string& devPath) {
  unsigned id{};

#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)

  struct _stat s;
  auto ret = _stat(devPath.c_str(), &s);
  if (0 == ret) 
    id = static_cast<unsigned>(s.st_dev);

#else

  struct stat s;
  auto ret = stat(devPath.c_str(), &s);
  if (-1 != ret) 
    id = static_cast<unsigned>(minor(s.st_dev) / 16);

#endif

  return id;
}

/**
 *  \brief _rpc.txt | .rpb | .rpc
 * 
 *  \param [in] imgFile path of the image file
 */
std::string FindRationalModelFile(const std::string& imgFile) {
  return {};
}
} // utils
} // earthxpert
