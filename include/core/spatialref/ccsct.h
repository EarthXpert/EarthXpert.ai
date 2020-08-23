/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/ict.h"
#include "3rdparty/geotrans/exgeotrans.h"

#include <memory>

namespace earthxpert {
/**
 *  The class CcsCT provides service of coordinate transformation
 *  from a source coordinate system to a target coordinate system
 *  with OGR as backend.
 */
class EXCORE_EXPORTS CcsCT : public ICT {
public:
  CcsCT();
  ~CcsCT() = default;

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

  /**
   *  \brief Transform an array of points in-place.
   *
   *  \param [in] points an array of 3D points
   *  \return Returns if the transformation was successful, otherwise false.
   */
  bool Transform(std::vector<Point3d>& points) override;

private:
  void Initialize();
};
} // namespace earthxpert
