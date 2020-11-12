/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/ict.h"
#include "core/spatialref/gdalsrs.h"

#include <memory>

namespace earthxpert {
/**
 *  The class GdalCT provides service of coordinate transformation 
 *  from a source coordinate system to a target coordinate system  
 *  with OGR as backend.
 */
class EXCORE_EXPORTS GdalCT final : public ICT {
public:
  GdalCT(const GdalSRS* src, const GdalSRS* tgt);
  GdalCT(OGRSpatialReference* src, OGRSpatialReference* tgt);
  GdalCT(const std::string& srcWkt, const std::string& tgtWkt);
  ~GdalCT() = default;

  /**
   *  \return Returns the backend OGRCoordinateTransformation object. 
   *  GdalCT exports partially the OGR functions. The caller could 
   *  exploit the full OGR capabilities via the returned object pointer.
   */
  OGRCoordinateTransformation* GetCT() { return mCT.get(); }

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
    return mCT->Transform(1, &x, &y, &z);
  }

  bool Transform(const double xSrc, const double ySrc, const double zSrc,
    double& xTgt, double& yTgt, double& zTgt) override {
    xTgt = xSrc; yTgt = ySrc; zTgt = zSrc;
    return mCT->Transform(1, &xTgt, &yTgt, &zTgt);
  }

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] point a 3D point with XYZ coordinates
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(Point3d& point) override {
    return mCT->Transform(1, &point.x, &point.y, &point.z);
  }

  bool Transform(const Point3d& src, Point3d& tgt) override {
    tgt = src;
    return mCT->Transform(1, &tgt.x, &tgt.y, &tgt.z);
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

  bool Transform(const std::vector<Point3d>& src,
    std::vector<Point3d>& tgt) override {
    tgt = src;
    return Transform(tgt);
  }

private:
  std::unique_ptr<OGRCoordinateTransformation> mCT;

  void Initialize(OGRSpatialReference* src, OGRSpatialReference* tgt);
};

using UPGdalCT = std::unique_ptr<GdalCT>;
using SPGdalCT = std::shared_ptr<GdalCT>;
} // namespace earthxpert
