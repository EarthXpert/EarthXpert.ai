/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/spatialref//spatialdef.h"

namespace earthxpert {
/**
 *  The interface IGroundPolicy defines the ground transformation
 *  between the model and Cartesian coordinates.
 */
class EXCORE_EXPORTS IGroundPolicy {
public:
  enum class GroundPolicyType {
    eGEOGRAPHIC, //
    eLOCAL, //
    eMAP //
  };
  using gpt = GroundPolicyType;

  virtual ~IGroundPolicy() {}

  /**
   *  \brief Given a 3d position in Cartesian coordinate system,
   *  output the 3d position in model ground coordinate system.
   *
   *  \param [in] rect 3d position in Cartesian coordinate system
   *  \return Return 3d position in model ground coordinate system
   */
  virtual Point3d CartesianToGround(const Point3d &rect) const = 0;

  /**
   *  \brief Given a 3d position in model ground coordinate system,
   *  output the 3d position in Cartesian coordinate system.
   *
   *  \param [in] ground 3d position in model ground coordinate system
   *  \return Return 3d position in Cartesian coordinate system
   */
  virtual Point3d GroundToCartesian(const Point3d &ground) const = 0;

  /**
   * \return Return type of ground policy
   */
  virtual GroundPolicyType GetGroundPolicyType() const noexcept = 0;
};
}