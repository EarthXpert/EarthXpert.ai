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
ProjCt::ProjCt(const ProjSrs* src, const ProjSrs* tgt) {
  Initialize(src->GetPJ(), tgt->GetPJ());
}

ProjCt::ProjCt(const PJ* src, const PJ* tgt) {
  Initialize(src, tgt);
}

ProjCt::ProjCt(const std::string& src, const std::string& tgt) {
  mCT.reset(proj_create_crs_to_crs(PJ_DEFAULT_CTX, 
    src.c_str(), tgt.c_str(), nullptr));
}

/**
 *  \remark ProjCt owns the objects of PJ?
 */
void ProjCt::Initialize(const PJ* src, const PJ* tgt) {
  mCT.reset(proj_create_crs_to_crs_from_pj(
    PJ_DEFAULT_CTX, src, tgt, nullptr, nullptr));
}
