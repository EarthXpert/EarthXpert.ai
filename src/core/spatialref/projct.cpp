/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/projct.h"

#include <omp.h>

using namespace earthxpert;
using namespace earthxpert::thirdparty;

/**
 * constructor.
 *
 * \param [in] src the source SRS
 * \param [in] tgt the target SRS
 */
ProjCT::ProjCT(const ProjSRS* src, const ProjSRS* tgt) {

}

/**
 *  \remark ProjCT owns the objects of OGRSpatialReference?
 */
void ProjCT::Initialize() {
}
