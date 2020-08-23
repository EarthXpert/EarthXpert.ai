/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"

#include <vector>

namespace earthxpert {
/**
 *  The class ICT provides service of coordinate transformation
 *  from a source coordinate system to a target coordinate system
 */
class EXCORE_EXPORTS ICT
{
public:
  virtual ~ICT() = default;

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] x x-coordinate
   *  \param [in] y y-coordinate
   *  \param [in] z z-coordinate
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  virtual bool Transform(double& x, double& y, double& z) = 0;

  /**
   *  \brief Transform a point in-place.
   *
   *  \param [in] point a 3D point with XYZ coordinates
   *
   *  \return Returns if the transformation was successful, otherwise false.
   */
  virtual bool Transform(Point3d& point) = 0;

  /**
   *  \brief Transform an array of points in-place.
   *
   *  \param [in] points an array of 3D points
   *  \return Returns if the transformation was successful, otherwise false.
   */
  virtual bool Transform(std::vector<Point3d>& points) = 0;
};
} // namespace earthxpert
