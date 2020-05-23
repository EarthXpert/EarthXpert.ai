/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/stringutils.h"

namespace earthxpert::utils {
/**
 *  \brief compare two string case-insensitive
 *
 *  \param [in] str1 first string
 *  \param [in] str2 second string
 *  \return Returns true if two strings are equal case-insensitive
 */
bool CompareI(const std::string& str1, const std::string& str2) {
  return ((str1.size() == str2.size()) &&
    std::equal(str1.begin(), str1.end(), str2.begin(),
      [](const char c1, const char c2) {
        return (c1 == c2 ||
          std::toupper(c1) == std::toupper(c2));
      }));
}
}