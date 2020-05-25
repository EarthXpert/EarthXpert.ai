/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/groundpolicy.h"

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
GeographicGround::GeographicGround(std::unique_ptr<EarthEllipsoid>&& earthEllip)
    : mEarthEllip(std::move(earthEllip)) {
  if (!mEarthEllip)
    throw;
}

GeographicGround::GeographicGround(const GeographicGround &rhs) {
  CopyFrom(rhs);
}

GeographicGround &GeographicGround::operator=(const GeographicGround &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

bool GeographicGround::operator==(const GeographicGround &rhs) const {
  return *mEarthEllip.get() == *(rhs.mEarthEllip.get());
}

bool GeographicGround::operator!=(const GeographicGround &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification LSR coordinate system,
 *  output the 3d position in model geographic coordinate system.
 *
 *  \param [in] rect 3d position in LSR coordinate system
 *  \return Return 3d position in geographic coordinate system
 */
Point3d GeographicGround::RectToGround(const Point3d &rect) const {
  return mEarthEllip->LSRToLPH(rect);
}

/**
 *  \brief Given a 3d position in model geographic coordinate system,
 *  output the 3d position in rectification LSR coordinate system.
 *
 *  \param [in] ground 3d position in geographic coordinate system
 *  \return Return 3d position in LSR coordinate system
 */
Point3d GeographicGround::GroundToRect(const Point3d &ground) const {
  return mEarthEllip->LPHToLSR(ground);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
void GeographicGround::CopyFrom(const GeographicGround &rhs) {
  mEarthEllip = rhs.mEarthEllip->Clone();
}

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
