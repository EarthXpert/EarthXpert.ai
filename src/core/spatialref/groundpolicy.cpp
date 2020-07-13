/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/spatialref/groundpolicy.h"

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
template <int linearUnit> LocalGround<linearUnit>::LocalGround(const Point3d& origin) 
  : mOrigin(origin) {
}

template <int linearUnit> 
LocalGround<linearUnit>::LocalGround(const LocalGround<linearUnit>&rhs) {}

template <int linearUnit> 
LocalGround<linearUnit>::LocalGround(LocalGround<linearUnit>&&rhs) noexcept {}

template <int linearUnit> LocalGround<linearUnit>& 
LocalGround<linearUnit>::operator=(const LocalGround<linearUnit>&rhs) {
  return *this;
}

template <int linearUnit> LocalGround<linearUnit>& 
LocalGround<linearUnit>::operator=(LocalGround<linearUnit>&&rhs) noexcept {
  return *this;
}

template <int linearUnit> 
bool LocalGround<linearUnit>::operator==(const LocalGround<linearUnit>&rhs) const {
  return true;
}

template <int linearUnit> 
bool LocalGround<linearUnit>::operator!=(const LocalGround<linearUnit>&rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in Cartesian LSR coordinate system,
 *  output the 3d position in model LSR coordinate system.
 *
 *  \param [in] rect 3d position in Cartesian LSR coordinate system
 *  \return Return 3d position in model LSR coordinate system
 */
template <int linearUnit> 
Point3d LocalGround<linearUnit>::CartesianToGround(const Point3d &rect) const {
  return (mOrigin + rect);
}

/**
 *  \brief Given a 3d position in model LSR coordinate system,
 *  output the 3d position in Cartesian LSR coordinate system.
 *
 *  \param [in] ground 3d position in model LSR coordinate system
 *  \return Return 3d position in Cartesian LSR coordinate system
 */
template <int linearUnit> 
Point3d LocalGround<linearUnit>::GroundToCartesian(const Point3d &ground) const {
  return (ground - mOrigin);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <int linearUnit> 
void LocalGround<linearUnit>::CopyFrom(const LocalGround &rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
template <int linearUnit> 
void LocalGround<linearUnit>::MoveFrom(LocalGround &&rhs) noexcept {
}

template class earthxpert::LocalGround<earthxpert::kLU_FEET>;
template class earthxpert::LocalGround<earthxpert::kLU_METER>;
template class earthxpert::LocalGround<earthxpert::kLU_USFEET>;

/**
 *  Constructor.
 *
 *  \brief Constructor.
 *
 *  \param [in]
 */
template <int linearUnit> MapGround<linearUnit>::MapGround(const Point3d& origin) 
  : mOrigin(origin) {
}

template <int linearUnit> 
MapGround<linearUnit>::MapGround(const MapGround<linearUnit>&rhs) {}

template <int linearUnit> 
MapGround<linearUnit>::MapGround(MapGround<linearUnit>&&rhs) noexcept {}

template <int linearUnit> MapGround<linearUnit>& 
MapGround<linearUnit>::operator=(const MapGround<linearUnit>&rhs) {
  return *this;
}

template <int linearUnit> MapGround<linearUnit>& 
MapGround<linearUnit>::operator=(MapGround<linearUnit>&&rhs) noexcept {
  return *this;
}

template <int linearUnit> 
bool MapGround<linearUnit>::operator==(const MapGround<linearUnit>&rhs) const {
  return true;
}

template <int linearUnit> 
bool MapGround<linearUnit>::operator!=(const MapGround<linearUnit>&rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in Cartesian map coordinate system,
 *  output the 3d position in model map coordinate system.
 *
 *  \param [in] rect 3d position in Cartesian map coordinate system
 *  \return Return 3d position in model map coordinate system
 */
template <int linearUnit> 
Point3d MapGround<linearUnit>::CartesianToGround(const Point3d &rect) const {
  return (mOrigin + rect);
}

/**
 *  \brief Given a 3d position in model map coordinate system,
 *  output the 3d position in Cartesian map coordinate system.
 *
 *  \param [in] ground 3d position in model map coordinate system
 *  \return Return 3d position in Cartesian map coordinate system
 */
template <int linearUnit> 
Point3d MapGround<linearUnit>::GroundToCartesian(const Point3d &ground) const {
  return (ground - mOrigin);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <int linearUnit> 
void MapGround<linearUnit>::CopyFrom(const MapGround<linearUnit>&rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
template <int linearUnit> 
void MapGround<linearUnit>::MoveFrom(MapGround<linearUnit>&&rhs) noexcept  {
}

template class earthxpert::MapGround<earthxpert::kLU_FEET>;
template class earthxpert::MapGround<earthxpert::kLU_METER>;
template class earthxpert::MapGround<earthxpert::kLU_USFEET>;
