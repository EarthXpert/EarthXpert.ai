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
template <typename _Ty> class EXSENSORMODEL_EXPORTS DistortionGrid {
public:
  using ValueType = _Ty;

  DistortionGrid(std::string cameraName, const std::vector<_Ty> &vX,
                 const std::vector<_Ty> &vY, const std::vector<_Ty> &vdX,
                 const std::vector<_Ty> &vdY);
  DistortionGrid(const std::vector<_Ty> &vX, const std::vector<_Ty> &vY,
                 const std::vector<_Ty> &vdX, const std::vector<_Ty> &vdY);
  DistortionGrid(const DistortionGrid &rhs);
  DistortionGrid(DistortionGrid &&rhs) noexcept;
  ~DistortionGrid() = default;

  DistortionGrid &operator=(const DistortionGrid &rhs);
  DistortionGrid &operator=(DistortionGrid &&rhs) noexcept;

  bool operator==(const DistortionGrid &rhs) const;
  bool operator!=(const DistortionGrid &rhs) const;

  bool FetchGrid(unsigned int &numCols, unsigned int &numRows,
                 std::vector<_Ty> &vX, std::vector<_Ty> &vY,
                 std::vector<_Ty> &vdX, std::vector<_Ty> &vdY) const;

  bool GetDistortion(_Ty X, _Ty Y, _Ty *const d) const;
  std::pair<_Ty, _Ty> GetDistortion(_Ty X, _Ty Y) const;

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
  std::unique_ptr<InterpGrid<_Ty>> mGrid;

  // void VerifyGrid(const std::vector<_Ty>& vX, const std::vector<_Ty>& vY)
  // const;
  void EstablishGrid(const std::vector<_Ty> &vX, const std::vector<_Ty> &vY,
                     const std::vector<_Ty> &vdX, const std::vector<_Ty> &vdY);

  void CopyFrom(const DistortionGrid &rhs);
  void MoveFrom(DistortionGrid &&rhs) noexcept;
};

using DistortionGridF = DistortionGrid<float>;
using DistortionGridD = DistortionGrid<double>;
// using DistortionGridLD = DistortionGrid<long double>;
} // namespace earthxpert
