/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/ict.h"
#include "core/spatialref/projsrs.h"

#include <memory>

namespace earthxpert {
/**
 *  The class ProjCT provides service of coordinate transformation
 *  from a source coordinate system to a target coordinate system
 *  with OGR as backend.
 */
class EXCORE_EXPORTS ProjCT final : public ICT {
public:
  ProjCT(const ProjSRS* src, const ProjSRS* tgt);
  ProjCT(const PJ* src, const PJ* tgt);
  ~ProjCT() = default;

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] x x-coordinate
   *  \param [in] y y-coordinate
   *  \param [in] z z-coordinate
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(double& x, double& y, double& z) override {
    const auto tgt = proj_trans(mCT.get(), PJ_FWD, PJ_COORD{ x, y, z, 0.0 });
    std::tie(x, y, z) = GetXYZ(tgt);
    return true;
  }

  bool Transform(const double srcX, const double srcY, const double srcZ,
    double& tgtX, double& tgtY, double& tgtZ) override {
    const auto tgt = proj_trans(mCT.get(), PJ_FWD, PJ_COORD{ srcX, srcY, srcZ, 0.0 });
    std::tie(tgtX, tgtY, tgtZ) = GetXYZ(tgt);
    return true;
  }

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] point a 3D point with XYZ coordinates
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(Point3d& point) override {
    return Transform(point.x, point.y, point.z);
  }

  bool Transform(const Point3d& src, Point3d& tgt) override {
    return Transform(src.x, src.y, src.z, tgt.x, tgt.y, tgt.z);
  }

  /**
   *  \brief Transform an array of points in-place.
   *
   *  \param [in] points an array of 3D points
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(std::vector<Point3d>& points) override {
    bool ret{ true };
    for (auto& point : points) {
      ret = ret && Transform(point);
    }

    return ret;
  }

  bool Transform(const std::vector<Point3d>& src, std::vector<Point3d>& tgt) {
    const size_t numPts = src.size();
    tgt.resize(numPts);

    bool ret{ true };
    for (size_t i = 0; i < numPts; ++i) {
      ret = ret && Transform(src[i], tgt[i]);
    }

    return ret;
  }

private:
  UPPJ mCT;

  void Initialize(const PJ* src, const PJ* tgt);
};

using UPProjCT = std::unique_ptr<ProjCT>;
using SPProjCT = std::shared_ptr<ProjCT>;
} // namespace earthxpert
