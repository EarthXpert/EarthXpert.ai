/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/ict.h"
#include "core/spatialref/ccssrs.h"

namespace earthxpert {
/**
 *  The class CcsCt provides service of coordinate transformation
 *  from a source coordinate system to a target coordinate system
 *  with Geotrans & dtcc as backend.
 */
class EXCORE_EXPORTS CcsCt : public ICt {
public:
  CcsCt(const CcsSrs* src, const CcsSrs* tgt);
  ~CcsCt() = default;

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
    return false;
  }

  bool Transform(const double srcX, const double srcY, const double srcZ,
    double& tgtX, double& tgtY, double& tgtZ) {
    return false;
  }

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] point a 3D point with XYZ coordinates
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(Point3d& point) override {
    return false;
  }

  bool Transform(const Point3d& src, Point3d& tgt) override {
    return false;
  }

  /**
   *  \brief Transform an array of points in-place.
   *
   *  \param [in] points an array of 3D points
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(std::vector<Point3d>& points) override {
    return false;
  }

  bool Transform(const std::vector<Point3d>& src,
    std::vector<Point3d>& tgt) override {
    return false;
  }

private:
  void Initialize();
};

using UPCcsCt = std::unique_ptr<CcsCt>;
using SPCcsCt = std::shared_ptr<CcsCt>;
} // namespace earthxpert
