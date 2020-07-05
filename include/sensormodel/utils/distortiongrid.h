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
 *  Encapsulates a calibrated grid of camera lens distortion.
 */
template <typename Ty> class EXSENSORMODEL_EXPORTS DistortionGrid {
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
   * Interpolate a distortion amount at the given X,Y position.
   *
   * \param [in] X X position
   * \param [in] Y Y position
   * \param [in,out] d Array of interpolated (or extrapolated) distortions
   */
  bool GetDistortion(const Ty X, const Ty Y,
    Ty* const d) const {
    if (mGrid) {
      assert(d);
      mGrid->GetVal(X, Y, d);
      return true;
    }
    return false;
  }

  /**
   * Interpolate a distortion value for the given X,Y position.
   *
   * \param [in] X X position
   * \param [in] Y Y position
   * \return Returns interpolated (or extrapolated) X & Y distortions
   */
  std::pair<Ty, Ty> GetDistortion(const Ty X, const Ty Y) const {
    if (mGrid) {
      assert(2 == mGrid->GetDimension());
      return mGrid->GetVal2(X, Y);
    }
    return { 0.f, 0.f };
  }

  std::unique_ptr<DistortionGrid> Clone() const {
    return std::make_unique<DistortionGrid>(*this);
  }

  void SetCameraName(std::string cameraName) {
    mCameraName = std::move(cameraName);
  }
  std::string GetCameraName() const noexcept { return mCameraName; }
  bool HasGrid() const noexcept { return bool(mGrid); }

private:
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
} // namespace earthxpert
