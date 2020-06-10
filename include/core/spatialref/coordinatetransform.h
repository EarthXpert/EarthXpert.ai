/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/gdal/exgdal.h"
#include "3rdparty/opencv/exopencv.h"

#include <memory>
#include <vector>

namespace earthxpert {
/**
 *  The class CoordinateTransform provides service of coordinate 
 *  transformation from a source coordinate system to a target coordinate system  
 *  with OGR as backend.
 */
class EXCORE_EXPORTS CoordinateTransform
{
public:
  CoordinateTransform(OGRSpatialReference* srcSrs, OGRSpatialReference* tgtSrs);
  CoordinateTransform(const std::string& srcWkt, const std::string& tgtWkt);
  ~CoordinateTransform() = default;

  /**
   *  \return Returns the backend OGRCoordinateTransformation object. 
   *  CoordinateTransform exports partially the OGR functions. The caller could 
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
  inline bool Transform(double& x, double& y, double& z) {
    return mCT->Transform(1, &x, &y, &z);
  }

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] point a 3D point with XYZ coordinates
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  inline bool Transform(Point3d& point) {
    return mCT->Transform(1, &point.x, &point.y, &point.z);
  }

  /**
   *  \brief Transform an array of points in-place.
   *
   *  \param [in] points an array of 3D points
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(std::vector<Point3d>& points);

private:
  std::unique_ptr<OGRCoordinateTransformation> mCT;

  void Initialize(OGRSpatialReference* srcSrs, OGRSpatialReference* tgtSrs);
};

} // namespace pdal

