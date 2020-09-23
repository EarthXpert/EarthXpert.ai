/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/stringutils.h"

#include <algorithm>

namespace earthxpert::utils {
/**
 *  \brief compare two string case-insensitive
 *
 *  \param [in] s1 first string
 *  \param [in] s2 second string
 *  \return Returns true if two strings are equal case-insensitive
 */
bool CompareI(const std::string& s1, const std::string& s2) {
  return ((s1.size() == s2.size()) &&
    std::equal(s1.begin(), s1.end(), s2.begin(),
      [](const char c1, const char c2) {
        return (c1 == c2 ||
          std::toupper(c1) == std::toupper(c2));
      }));
}

/**
 *  \param [in] 
 *  \return Returns length of longest common substring
 */
int lcs(const char* s1, const int m, const char* s2, const int n, int count) {
  if (0 == m || 0 == n)
    return count;

  if (s1[m - 1] == s2[n - 1]) {
    count = lcs(s1, m - 1, s2, n - 1, count + 1);
  }

  return std::max(count, 
    std::max(lcs(s1, m, s2, n - 1, 0), lcs(s1, m - 1, s2, n, 0)));
}
}
