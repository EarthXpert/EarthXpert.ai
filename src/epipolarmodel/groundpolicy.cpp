/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/groundpolicy.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/unitutils.h"

#include <cassert>
#include <utility>

using earthxpert::EarthEllipsoid;
using earthxpert::GeographicGround;
using earthxpert::LocalGround;
using earthxpert::MapGround;
using earthxpert::Point2d;
using earthxpert::Point3d;

/**
 *  Constructor.
 *
 *  \brief Constructor.
 *
 *  \param [in] 
 */
template <int AngleUnit>
GeographicGround<AngleUnit>::GeographicGround(std::unique_ptr<EarthEllipsoid>&& earthEllip)
    : mEarthEllip(std::move(earthEllip)) {
  if (!mEarthEllip)
    throw std::invalid_argument("GeographicGround: earthEllip");
}

template <int AngleUnit>
GeographicGround<AngleUnit>::GeographicGround(const GeographicGround &rhs) {
  CopyFrom(rhs);
}

template <int AngleUnit>
GeographicGround<AngleUnit>& GeographicGround<AngleUnit>::operator=(const GeographicGround<AngleUnit>&rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

template <int AngleUnit>
bool GeographicGround<AngleUnit>::operator==(const GeographicGround<AngleUnit>&rhs) const {
  return *mEarthEllip.get() == *(rhs.mEarthEllip.get());
}

template <int AngleUnit>
bool GeographicGround<AngleUnit>::operator!=(const GeographicGround<AngleUnit>&rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification LSR coordinate system,
 *  output the 3d position in model geographic coordinate system.
 *
 *  \param [in] rect 3d position in LSR coordinate system
 *  \return Return 3d position in geographic coordinate system
 */
template <int AngleUnit>
Point3d GeographicGround<AngleUnit>::RectToGround(const Point3d &rect) const {
  if constexpr (kAU_DEGREE == AngleUnit) {
    return Rad2Deg(mEarthEllip->LSRToLPH(rect));
  }
  else if constexpr (kAU_GON == AngleUnit) {
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
 *  \param [in] ground 3d position in geographic coordinate system
 *  \return Return 3d position in LSR coordinate system
 */
template <int AngleUnit>
Point3d GeographicGround<AngleUnit>::GroundToRect(const Point3d &ground) const {
  if constexpr (kAU_DEGREE == AngleUnit) {
    return mEarthEllip->LSRToLPH(Deg2Rad(ground));
  }
  else if constexpr (kAU_GON == AngleUnit) {
    return mEarthEllip->LSRToLPH(Gon2Rad(ground));
  }
  else {
    return mEarthEllip->LPHToLSR(ground);
  }
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <int AngleUnit>
void GeographicGround<AngleUnit>::CopyFrom(const GeographicGround &rhs) {
  mEarthEllip = rhs.mEarthEllip->Clone();
}

template class earthxpert::GeographicGround<earthxpert::kAU_DEGREE>;
template class earthxpert::GeographicGround<earthxpert::kAU_GON>;
template class earthxpert::GeographicGround<earthxpert::kAU_RADIAN>;

/**
 *  Constructor.
 *
 *  \brief Constructor.
 *
 *  \param [in]
 */
LocalGround::LocalGround() {
}

LocalGround::LocalGround(const LocalGround &rhs) {}

LocalGround::LocalGround(LocalGround &&rhs) noexcept {}

LocalGround &LocalGround::operator=(const LocalGround &rhs) {
  return *this;
}

LocalGround &LocalGround::operator=(LocalGround &&rhs) noexcept {
  return *this;
}

bool LocalGround::operator==(const LocalGround &rhs) const {
  return true;
}

bool LocalGround::operator!=(const LocalGround &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification LSR coordinate system,
 *  output the 3d position in model LSR coordinate system.
 *
 *  \param [in] rect 3d position in rectification LSR coordinate system
 *  \return Return 3d position in model LSR coordinate system
 */
Point3d LocalGround::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model LSR coordinate system,
 *  output the 3d position in rectification LSR coordinate system.
 *
 *  \param [in] ground 3d position in model LSR coordinate system
 *  \return Return 3d position in rectification LSR coordinate system
 */
Point3d LocalGround::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
void LocalGround::CopyFrom(const LocalGround &rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
void LocalGround::MoveFrom(LocalGround &&rhs) noexcept {
}

/**
 *  Constructor.
 *
 *  \brief Constructor.
 *
 *  \param [in]
 */
MapGround::MapGround() {
}

MapGround::MapGround(const MapGround &rhs) {}

MapGround::MapGround(MapGround &&rhs) noexcept {}

MapGround &MapGround::operator=(const MapGround &rhs) {
  return *this;
}

MapGround &MapGround::operator=(MapGround &&rhs) noexcept {
  return *this;
}

bool MapGround::operator==(const MapGround &rhs) const {
  return true;
}

bool MapGround::operator!=(const MapGround &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification map coordinate system,
 *  output the 3d position in model map coordinate system.
 *
 *  \param [in] rect 3d position in rectification map coordinate system
 *  \return Return 3d position in model map coordinate system
 */
Point3d MapGround::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model map coordinate system,
 *  output the 3d position in rectification map coordinate system.
 *
 *  \param [in] ground 3d position in model map coordinate system
 *  \return Return 3d position in rectification map coordinate system
 */
Point3d MapGround::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
void MapGround::CopyFrom(const MapGround &rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
void MapGround::MoveFrom(MapGround &&rhs) noexcept  {
}
