/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/ccsct.h"

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] src the source SRS
 * \param [in] tgt the target SRS
 */
CcsCT::CcsCT(const CcsSRS* src, const CcsSRS* tgt) {
  Initialize();
}

void CcsCT::Initialize() {
}
