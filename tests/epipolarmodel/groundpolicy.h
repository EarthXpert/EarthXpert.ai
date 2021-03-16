/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/igroundpolicy.h"
#include "core/spatialref/earthellipsoid.h"

namespace watxtract {
/**
 *  The class GeographicGround offers the ground transformation
 *  between the geographic and LSR coordinates.
 */
class WXEPIPOLARMODEL_EXPORTS GeographicGround final : public IGroundPolicy {
public:
  /**
   *  \brief Constructor.
   *
   *  \param [in] 
   */
  GeographicGround(std::unique_ptr<EarthEllipsoid>&& earthEllip);
  GeographicGround(const GeographicGround &rhs);
  GeographicGround(GeographicGround &&rhs) noexcept = default;
  ~GeographicGround() = default;

  GeographicGround &operator=(const GeographicGround &rhs);
  GeographicGround &operator=(GeographicGround &&rhs) noexcept = default;

  bool operator==(const GeographicGround &rhs) const;
  bool operator!=(const GeographicGround &rhs) const;

  /**
   *  \brief Given a 3d position in rectification LSR coordinate system,
   *  output the 3d position in model geographic coordinate system.
   *
   *  \param [in] rect 3d position in LSR coordinate system
   *  \return Return 3d position in geographic coordinate system
   */
  Point3d RectToGround(const Point3d &rect) const  override;

  /**
   *  \brief Given a 3d position in model geographic coordinate system,
   *  output the 3d position in rectification LSR coordinate system.
   *
   *  \param [in] ground 3d position in geographic coordinate system
   *  \return Return 3d position in LSR coordinate system
   */
  Point3d GroundToRect(const Point3d &ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eGEOGRAPHIC;
  }

private:
  std::unique_ptr<EarthEllipsoid> mEarthEllip;

  void CopyFrom(const GeographicGround &rhs);
};

/**
 *  The class LocalGround offers the ground transformation
 *  between the model LSR and the rectification LSR coordinates.
 */
class WXEPIPOLARMODEL_EXPORTS LocalGround final : public IGroundPolicy{
public:
  /**
   *  \brief Constructor.
   *
   *  \param [in]
   */
  LocalGround();

  LocalGround(const LocalGround &rhs);
  LocalGround(LocalGround &&rhs) noexcept;
  ~LocalGround() = default;

  LocalGround &operator=(const LocalGround &rhs);
  LocalGround &operator=(LocalGround &&rhs) noexcept;

  bool operator==(const LocalGround &rhs) const;
  bool operator!=(const LocalGround &rhs) const;

  /**
   *  \brief Given a 3d position in rectification LSR coordinate system,
   *  output the 3d position in model LSR coordinate system.
   *
   *  \param [in] rect 3d position in rectification LSR coordinate system
   *  \return Return 3d position in model LSR coordinate system
   */
  Point3d RectToGround(const Point3d &rect) const  override;

  /**
   *  \brief Given a 3d position in model LSR coordinate system,
   *  output the 3d position in rectification LSR coordinate system.
   *
   *  \param [in] ground 3d position in model LSR coordinate system
   *  \return Return 3d position in rectification LSR coordinate system
   */
  Point3d GroundToRect(const Point3d &ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eLOCAL;
  }

private:
  void CopyFrom(const LocalGround &rhs);
  void MoveFrom(LocalGround &&rhs) noexcept;
};

/**
 *  The class MapGround offers the ground transformation
 *  between the model map and the rectification map coordinates.
 */
class WXEPIPOLARMODEL_EXPORTS MapGround final : public IGroundPolicy {
public:
  /**
   *  \brief Constructor.
   *
   *  \param [in]
   */
  MapGround();

  MapGround(const MapGround &rhs);
  MapGround(MapGround &&rhs) noexcept;
  ~MapGround() = default;

  MapGround &operator=(const MapGround &rhs);
  MapGround &operator=(MapGround &&rhs) noexcept;

  bool operator==(const MapGround &rhs) const;
  bool operator!=(const MapGround &rhs) const;

  /**
   *  \brief Given a 3d position in rectification map coordinate system,
   *  output the 3d position in model map coordinate system.
   *
   *  \param [in] rect 3d position in rectification map coordinate system
   *  \return Return 3d position in model map coordinate system
   */
  Point3d RectToGround(const Point3d &rect) const  override;

  /**
   *  \brief Given a 3d position in model map coordinate system,
   *  output the 3d position in rectification map coordinate system.
   *
   *  \param [in] ground 3d position in model map coordinate system
   *  \return Return 3d position in rectification map coordinate system
   */
  Point3d GroundToRect(const Point3d &ground) const override;

  /**
   * \return Return type of ground policy
   */
  GroundPolicyType GetGroundPolicyType() const noexcept override {
    return gpt::eMAP;
  }

private:
  void CopyFrom(const MapGround &rhs);
  void MoveFrom(MapGround &&rhs) noexcept;
};
} // namespace watxtract
