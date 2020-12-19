/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/math/interpgrid.h"
#include "sensormodel/exsensormodel.h"

#include <string>

namespace earthxpert {
/**
 *  \brief Encapsulates a calibrated grid of camera lens distortion.
 *
 *  \tparam Ty value type such as float, double
 *  \todo represent node position suing Point_<Ty> 
 */
template <typename Ty> class EXSENSORMODEL_EXPORTS DistortionGrid {
  static_assert(std::is_floating_point_v<Ty>, "Requires that template argument be floating data type");

public:
  using ValueType = Ty;

  DistortionGrid(std::string cameraName, const std::vector<Ty> &vX,
                 const std::vector<Ty> &vY, const std::vector<Ty> &vdX,
                 const std::vector<Ty> &vdY);
  DistortionGrid(const std::vector<Ty> &vX, const std::vector<Ty> &vY,
                 const std::vector<Ty> &vdX, const std::vector<Ty> &vdY);
  DistortionGrid(const DistortionGrid &rhs);
  DistortionGrid(DistortionGrid &&rhs) noexcept;
  ~DistortionGrid() = default;

  DistortionGrid &operator=(const DistortionGrid &rhs);
  DistortionGrid &operator=(DistortionGrid &&rhs) noexcept;

  bool operator==(const DistortionGrid &rhs) const;
  bool operator!=(const DistortionGrid &rhs) const;

  bool FetchGrid(unsigned int &numCols, unsigned int &numRows,
                 std::vector<Ty> &vX, std::vector<Ty> &vY,
                 std::vector<Ty> &vdX, std::vector<Ty> &vdY) const;

  /**
   * Interpolate a distortion amount at the given x,y position.
   *
   * \param [in] x x position
   * \param [in] y y position
   * \param [in,out] d Array of interpolated (or extrapolated) distortions
   */
  bool GetDistortion(const Ty x, const Ty y, Ty* const d) const;

  /**
   * Interpolate a distortion value for the given x,y position.
   *
   * \param [in] x x position
   * \param [in] y y position
   * \return Returns interpolated (or extrapolated) x & y distortions
   */
  std::pair<Ty, Ty> GetDistortion(const Ty x, const Ty y) const;

  std::unique_ptr<DistortionGrid> Clone() const {
    return std::make_unique<DistortionGrid>(*this);
  }

  void SetCameraName(std::string cameraName) {
    mCameraName = std::move(cameraName);
  }
  std::string GetCameraName() const noexcept { return mCameraName; }
  bool HasGrid() const noexcept { return bool(mGrid); }

private:
  friend class DistGridMarshaller;

  std::string mCameraName;
  std::unique_ptr<InterpGrid<Ty>> mGrid;

  // void VerifyGrid(const std::vector<Ty>& vX, const std::vector<Ty>& vY)
  // const;
  void EstablishGrid(const std::vector<Ty> &vX, const std::vector<Ty> &vY,
                     const std::vector<Ty> &vdX, const std::vector<Ty> &vdY);

  void CopyFrom(const DistortionGrid &rhs);
  void MoveFrom(DistortionGrid &&rhs) noexcept;
};

using DistortionGridF = DistortionGrid<float>;
using DistortionGridD = DistortionGrid<double>;
// using DistortionGridLD = DistortionGrid<long double>;

#include "detail/distortiongrid.hh"
} // namespace earthxpert
