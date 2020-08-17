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
 *  \param [in] s1 first string
 *  \param [in] s2 second string
 *  \return Returns true if two strings are equal case-insensitive
 */
bool EXCORE_EXPORTS CompareI(const std::string& s1, const std::string& s2);

int EXCORE_EXPORTS lcs(const char* s1, int m, const char* s2, int n, int count);
} // namespace earthxpert::utils
