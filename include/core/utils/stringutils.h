/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"
#include <string>

namespace watxtract::utils {
/**
 *  \brief compare two string case-insensitive
 *
 *  \param [in] str1 first string
 *  \param [in] str2 second string
 *  \return Returns true if two strings are equal case-insensitive
 */
bool WXCORE_EXPORTS CompareI(const std::string& str1, const std::string& str2);
} // namespace watxtract::utils
