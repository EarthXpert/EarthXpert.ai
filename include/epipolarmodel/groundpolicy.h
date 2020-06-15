/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/igroundpolicy.h"
#include "core/exceptions/exexcept.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/utils/unitutils.h"

#include <cassert>
#include <utility>

namespace earthxpert {
// angle unit
constexpr int kAU_DEGREE = 0;
constexpr int kAU_GON = 1;
constexpr int kAU_RADIAN = 2;

// linear unit
constexpr int kLU_FEET = 0;
constexpr int kLU_METER = 2;
constexpr int kLU_USFEET = 3;

/**
 *  The class GeographicGround offers the ground transformation
 *  between the geographic and LSR coordinates.
 *
 *  \param angleUnit angle unit of horizontal coordinates in ground: degree, gon, radian
 *  \param linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int angleUnit = kAU_RADIAN, int linearUnit = kLU_METER>
class EXEPIPOLARMODEL_EXPORTS GeographicGround final : public IGroundPolicy {
public:
  static constexpr SrsType kGroundType = eGEO;

  /**
   *  \brief Constructor.
   *
   *  \param [in]
   */
  GeographicGround(std::unique_ptr<EarthEllipsoid>&& earthEllip)
    : mEarthEllip(std::move(earthEllip)) {
    if (!mEarthEllip)
      throw std::invalid_argument("GeographicGround: earthEllip");
  }

  GeographicGround(const GeographicGround &rhs) {
    CopyFrom(rhs);
  }

  GeographicGround(GeographicGround &&rhs) noexcept = default;
  ~GeographicGround() = default;

  GeographicGround &operator=(const GeographicGround &rhs) {
    if (this != &rhs) {
      CopyFrom(rhs);
    }
    return *this;
  }

  GeographicGround &operator=(GeographicGround &&rhs) noexcept = default;

  bool operator==(const GeographicGround &rhs) const {
    return *mEarthEllip.get() == *(rhs.mEarthEllip.get());
  }

  bool operator!=(const GeographicGround &rhs) const {
    return !(operator==(rhs));
  }

  /**
   *  \brief Given a 3d position in rectification LSR coordinate system,
   *  output the 3d position in model geographic coordinate system.
   *
   *  \param [in] rect 3d position in LSR coordinate system
   *  \return Return 3d position in geographic coordinate system [unit: AngleUnit]
   */
  Point3d RectToGround(const Point3d &rect) const  override {
    if constexpr (kAU_DEGREE == angleUnit) {
      return Rad2Deg(mEarthEllip->LSRToLPH(rect));
    }
    else if constexpr (kAU_GON == angleUnit) {
      return Rad2Gon(mEarthEllip->LSRToLPH(rect));
    }
    else {
      return mEarthEllip->LSRToLPH(rect);
    }
  }

  /**
   *  \brief Given a 3d position in model geographic coordinate system,
   *  output the 3d position in rectification LSR coordinate system.
   *
   *  \param [in] ground 3d position in geographic coordinate system [unit: AngleUnit]
   *  \return Return 3d position in LSR coordinate system
   */
  Point3d GroundToRect(const Point3d &ground) const override {
    if constexpr (kAU_DEGREE == angleUnit) {
      return mEarthEllip->LSRToLPH(Deg2Rad(ground));
    }
    else if constexpr (kAU_GON == angleUnit) {
      return mEarthEllip->LSRToLPH(Gon2Rad(ground));
    }
    else {
      return mEarthEllip->LPHToLSR(ground);
    }
  }

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eGEOGRAPHIC;
  }

private:
  std::unique_ptr<EarthEllipsoid> mEarthEllip;

  void CopyFrom(const GeographicGround &rhs) {
    mEarthEllip = rhs.mEarthEllip->Clone();
  }
};

using GeographicGroundDeg = GeographicGround<kAU_DEGREE>;
using GeographicGroundGon = GeographicGround<kAU_GON>;
using GeographicGroundRad = GeographicGround<kAU_RADIAN>;

/**
 *  The class LocalGround offers the ground transformation
 *  between the model LSR and the rectification LSR coordinates.
 *
 *  \param linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int linearUnit = kLU_METER>
class EXEPIPOLARMODEL_EXPORTS LocalGround final : public IGroundPolicy{
public:
  static constexpr SrsType kGroundType = eLOCAL;

  /**
   *  \brief Constructor.
   *
   *  \param [in]
   */
  LocalGround();

  LocalGround(const LocalGround& rhs);
  LocalGround(LocalGround&& rhs) noexcept;
  ~LocalGround() = default;

  LocalGround& operator=(const LocalGround& rhs);

  LocalGround& operator=(LocalGround&& rhs) noexcept;

  bool operator==(const LocalGround& rhs) const;

  bool operator!=(const LocalGround& rhs) const;

  /**
   *  \brief Given a 3d position in rectification LSR coordinate system,
   *  output the 3d position in model LSR coordinate system.
   *
   *  \param [in] rect 3d position in rectification LSR coordinate system
   *  \return Return 3d position in model LSR coordinate system
   */
  Point3d RectToGround(const Point3d& rect) const  override;

  /**
   *  \brief Given a 3d position in model LSR coordinate system,
   *  output the 3d position in rectification LSR coordinate system.
   *
   *  \param [in] ground 3d position in model LSR coordinate system
   *  \return Return 3d position in rectification LSR coordinate system
   */
  Point3d GroundToRect(const Point3d& ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eLOCAL;
  }

private:
  void CopyFrom(const LocalGround& rhs);
  void MoveFrom(LocalGround&& rhs) noexcept;
};

using LocalGroundFt = LocalGround<kLU_FEET>;
using LocalGroundMe = LocalGround<kLU_METER>;
using LocalGroundUf = LocalGround<kLU_USFEET>;

/**
 *  The class MapGround offers the ground transformation
 *  between the model map and the rectification map coordinates.
 *
 *  \param linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int linearUnit = kLU_METER>
class EXEPIPOLARMODEL_EXPORTS MapGround final : public IGroundPolicy {
public:
  static constexpr SrsType kGroundType = eMAP;

  /**
   *  \brief Constructor.
   *
   *  \param [in]
   */
  MapGround();

  MapGround(const MapGround& rhs);
  MapGround(MapGround&& rhs) noexcept;
  ~MapGround() = default;

  MapGround& operator=(const MapGround& rhs);

  MapGround& operator=(MapGround&& rhs) noexcept;

  bool operator==(const MapGround& rhs) const;

  bool operator!=(const MapGround& rhs) const;

  /**
   *  \brief Given a 3d position in rectification map coordinate system,
   *  output the 3d position in model map coordinate system.
   *
   *  \param [in] rect 3d position in rectification map coordinate system
   *  \return Return 3d position in model map coordinate system
   */
  Point3d RectToGround(const Point3d& rect) const  override;

  /**
   *  \brief Given a 3d position in model map coordinate system,
   *  output the 3d position in rectification map coordinate system.
   *
   *  \param [in] ground 3d position in model map coordinate system
   *  \return Return 3d position in rectification map coordinate system
   */
  Point3d GroundToRect(const Point3d& ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eMAP;
  }

private:
  void CopyFrom(const MapGround& rhs);
  void MoveFrom(MapGround&& rhs) noexcept;
};

using MapGroundFt = MapGround<kLU_FEET>;
using MapGroundMe = MapGround<kLU_METER>;
using MapGroundUf = MapGround<kLU_USFEET>;
} // namespace earthxpert
