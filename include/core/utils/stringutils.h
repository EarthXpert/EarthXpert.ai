/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <string>

namespace earthxpert::utils {
/**
 *  \brief compare two string case-insensitive
 *
 *  \param [in] str1 first string
 *  \param [in] str2 second string
 *  \return Returns true if two strings are equal case-insensitive
 */
bool EXCORE_EXPORTS CompareI(const std::string& str1, const std::string& str2);

int EXCORE_EXPORTS lcs(const char* s1, int m, const char* s2, int n, int count);
} // namespace earthxpert::utils
