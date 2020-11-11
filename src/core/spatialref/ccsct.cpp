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
 * \param [in] srcSrs the source SRS
 * \param [in] tgtSrs the target SRS
 */
CcsCT::CcsCT(const CcsSRS* srcSrs, const CcsSRS* tgtSrs) {
  Initialize();
}

void CcsCT::Initialize() {
}
