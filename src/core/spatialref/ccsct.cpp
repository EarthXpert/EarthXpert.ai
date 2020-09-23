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
 * \param [in] srcWkt the source WKT
 * \param [in] tgtWkt the target WKT
 */
CcsCT::CcsCT() {
  Initialize();
}

void CcsCT::Initialize() {
}

/**
 *  \brief Transform an array of points in-place.
 *
 *  \param [in] points an array of 3D points
 *  \return Returns if the transformation was successful, otherwise false.
 */
bool CcsCT::Transform(std::vector<Point3d>& points) {
  bool ret{ true };
  for (auto& point : points) {
    ret = ret && Transform(point);
  }

  return ret;
}
