/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "blockadjust/exbba.h"

namespace earthxpert {
/** 
 *  \brief Adjustment parameters
 */
class EXBBA_EXPORTS BbaParam {
public:
    virtual ~BbaParam() = default;

    size_t mIndex{};          // Index in the normal equations
    double mInitVal{};        // Initial value
    double mInitStdDev{};     // Initial std deviation
    double mVal{};            // Current value
    double mStdDev{};         // Current std deviation

protected:
};
}
