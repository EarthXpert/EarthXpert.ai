/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/gdal/exgdal.h"
#include "sensormodel/exsensormodel.h"

#include <array>
#include <chrono>
#include <memory>

namespace earthxpert {
/**
 *  The class defines extrinsic parameters defining exterior orientation.
 *
 *  \detail
 */
class EXSENSORMODEL_EXPORTS FrameEP {
public:
  FrameEP(const std::array<double, 3> &position,
          const std::array<double, 3> &rotation,
          std::shared_ptr<OGRSpatialRef> groundSRS);

  FrameEP(const FrameEP &rhs);
  FrameEP(FrameEP &&rhs) noexcept;
  ~FrameEP() = default;

  FrameEP &operator=(const FrameEP &rhs);
  FrameEP &operator=(FrameEP &&rhs) noexcept;

  bool operator==(const FrameEP &rhs) const;
  bool operator!=(const FrameEP &rhs) const;

  auto GetPosition() const noexcept { return mPosition.data(); }

  auto GetRotation() const noexcept { return mRotation.data(); }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   */
  std::shared_ptr<OGRSpatialRef> GetCoordinateSystem() const
      noexcept {
    return mGroundSRS;
  }

  std::unique_ptr<FrameEP> Clone() const {
    return std::make_unique<FrameEP>(*this);
  }

private:
  // std::chrono::time_point mTimeStamp{};   // GPS time
  std::array<double, 3> mPosition{}; // position of body in world coordinate system
  std::array<double, 3> mRotation{}; // rotation angles of body in world
                                     // coordinate system [unit: degree]
  std::shared_ptr<OGRSpatialRef> mGroundSRS;

  // uncertainties
  std::array<double, 3> mStdPos{};
  std::array<double, 3> mStdRot{};

  void CheckSRS() const;

  void CopyFrom(const FrameEP &rhs);
  void MoveFrom(FrameEP &&rhs) noexcept;
};
} // namespace earthxpert
