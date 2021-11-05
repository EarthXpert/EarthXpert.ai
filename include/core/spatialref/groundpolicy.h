/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/spatialref/igroundpolicy.h"
#include "core/exceptions/exexcept.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/utils/unitutils.h"

#include <cassert>
#include <utility>

namespace earthxpert {
/**
 *  The class GeographicGround offers the ground transformation
 *  between the geographic and LSR coordinates.
 *
 *  \tparam angleUnit angle unit of horizontal coordinates in ground: degree, gon, radian
 *  \tparam linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int angleUnit = kAU_RADIAN, int linearUnit = kLU_METER>
class EXCORE_EXPORTS GeographicGround final : public IGroundPolicy {
public:
  static constexpr SrsType kGroundType = eST_GEO;

  /**
   *  \brief Constructor.
   *
   *  \param [in] earthEllip the local space rectangular system
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
   *  \brief Given a 3d position in Cartesian LSR coordinate system,
   *  output the 3d position in model geographic coordinate system.
   *
   *  \param [in] rect 3d position in LSR coordinate system
   *  \return Return 3d position in geographic coordinate system [unit: AngleUnit]
   */
  Point3d CartesianToGround(const Point3d &rect) const  override {
    Point3d lsr = LinearUnit<linearUnit>::ToMeter(rect);
    return AngleUnit<angleUnit>::FromRadian(mEarthEllip->LSRToLPH(lsr));
  }

  /**
   *  \brief Given a 3d position in model geographic coordinate system,
   *  output the 3d position in Cartesian LSR coordinate system.
   *
   *  \param [in] ground 3d position in geographic coordinate system [unit: AngleUnit]
   *  \return Return 3d position in LSR coordinate system
   */
  Point3d GroundToCartesian(const Point3d &ground) const override {
    Point3d lph = AngleUnit<angleUnit>::ToRadian(ground);
    return LinearUnit<linearUnit>::FromMeter(mEarthEllip->LPHToLSR(lph));
  }

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eGP_GEOGRAPHIC;
  }

  const EarthEllipsoid* GetEarthEllipsoid() const noexcept {
    return mEarthEllip.get();
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
 *  between the model LSR and the Cartesian LSR coordinates.
 *
 *  \tparam linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int linearUnit = kLU_METER>
class EXCORE_EXPORTS LocalGround final : public IGroundPolicy{
public:
  static constexpr SrsType kGroundType = eST_LOCAL;

  /**
   *  \brief Constructor.
   *
   *  \param [in] origin 
   */
  explicit LocalGround(const Point3d& origin);

  LocalGround(const LocalGround& rhs);
  LocalGround(LocalGround&& rhs) noexcept;
  ~LocalGround() = default;

  LocalGround& operator=(const LocalGround& rhs);
  LocalGround& operator=(LocalGround&& rhs) noexcept;

  bool operator==(const LocalGround& rhs) const;
  bool operator!=(const LocalGround& rhs) const;

  /**
   *  \brief Given a 3d position in Cartesian LSR coordinate system,
   *  output the 3d position in model LSR coordinate system.
   *
   *  \param [in] rect 3d position in Cartesian LSR coordinate system
   *  \return Return 3d position in model LSR coordinate system
   */
  Point3d CartesianToGround(const Point3d& rect) const override;

  /**
   *  \brief Given a 3d position in model LSR coordinate system,
   *  output the 3d position in Cartesian LSR coordinate system.
   *
   *  \param [in] ground 3d position in model LSR coordinate system
   *  \return Return 3d position in Cartesian LSR coordinate system
   */
  Point3d GroundToCartesian(const Point3d& ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eGP_LOCAL;
  }

private:
  // 3D similarity transformation
  Point3d mOrigin{ 0.0, 0.0, 0.0 }; // origin XYZ

  void CopyFrom(const LocalGround& rhs);
  void MoveFrom(LocalGround&& rhs) noexcept;
};

using LocalGroundFt = LocalGround<kLU_FOOT>;
using LocalGroundMe = LocalGround<kLU_METER>;
using LocalGroundUf = LocalGround<kLU_USFOOT>;

/**
 *  The class MapGround offers the ground transformation
 *  between the model map and the Cartesian map coordinates.
 *
 *  \tparam linearUnit linear unit of vertical coordinate in ground: feet, meter, us_survey_feet
 */
template <int linearUnit = kLU_METER>
class EXCORE_EXPORTS MapGround final : public IGroundPolicy {
public:
  static constexpr SrsType kGroundType = eST_MAP;

  /**
   *  \brief Constructor.
   *
   *  \param [in] origin 
   */
  explicit MapGround(const Point3d& origin);

  MapGround(const MapGround& rhs);
  MapGround(MapGround&& rhs) noexcept;
  ~MapGround() = default;

  MapGround& operator=(const MapGround& rhs);
  MapGround& operator=(MapGround&& rhs) noexcept;

  bool operator==(const MapGround& rhs) const;
  bool operator!=(const MapGround& rhs) const;

  /**
   *  \brief Given a 3d position in Cartesian map coordinate system,
   *  output the 3d position in model map coordinate system.
   *
   *  \param [in] rect 3d position in Cartesian map coordinate system
   *  \return Return 3d position in model map coordinate system
   */
  Point3d CartesianToGround(const Point3d& rect) const override;

  /**
   *  \brief Given a 3d position in model map coordinate system,
   *  output the 3d position in Cartesian map coordinate system.
   *
   *  \param [in] ground 3d position in model map coordinate system
   *  \return Return 3d position in Cartesian map coordinate system
   */
  Point3d GroundToCartesian(const Point3d& ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eGP_MAP;
  }

private:
  // 2D similarity transformation
  Point3d mOrigin{ 0.0, 0.0, 0.0 }; // origin XYZ

  void CopyFrom(const MapGround& rhs);
  void MoveFrom(MapGround&& rhs) noexcept;
};

using MapGroundIf = MapGround<kLU_FOOT>;
using MapGroundMe = MapGround<kLU_METER>;
using MapGroundUf = MapGround<kLU_USFOOT>;
} // namespace earthxpert
