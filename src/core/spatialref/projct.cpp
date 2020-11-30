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
  Initialize(src->GetPJ(), tgt->GetPJ());
}

ProjCT::ProjCT(const PJ* src, const PJ* tgt) {
  Initialize(src, tgt);
}

/**
 *  \remark ProjCT owns the objects of OGRSpatialReference?
 */
void ProjCT::Initialize(const PJ* src, const PJ* tgt) {
  mCT.reset(proj_create_crs_to_crs_from_pj(
    PJ_DEFAULT_CTX, src, tgt, nullptr, nullptr));
}
