/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/groundpolicy.h"

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
template <int linearUnit> LocalGround<linearUnit>::LocalGround() {
}

template <int linearUnit> 
LocalGround<linearUnit>::LocalGround(const LocalGround<linearUnit>&rhs) {}

template <int linearUnit> 
LocalGround<linearUnit>::LocalGround(LocalGround<linearUnit>&&rhs) noexcept {}

template <int linearUnit> LocalGround<linearUnit>& 
LocalGround<linearUnit>::operator=(const LocalGround<linearUnit>&rhs) {
  return *this;
}

template <int linearUnit> 
LocalGround<linearUnit>& LocalGround<linearUnit>::operator=(LocalGround<linearUnit>&&rhs) noexcept {
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
 *  \brief Given a 3d position in rectification LSR coordinate system,
 *  output the 3d position in model LSR coordinate system.
 *
 *  \param [in] rect 3d position in rectification LSR coordinate system
 *  \return Return 3d position in model LSR coordinate system
 */
template <int linearUnit> 
Point3d LocalGround<linearUnit>::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model LSR coordinate system,
 *  output the 3d position in rectification LSR coordinate system.
 *
 *  \param [in] ground 3d position in model LSR coordinate system
 *  \return Return 3d position in rectification LSR coordinate system
 */
template <int linearUnit> 
Point3d LocalGround<linearUnit>::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
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
template <int linearUnit> MapGround<linearUnit>::MapGround() {
}

template <int linearUnit> 
MapGround<linearUnit>::MapGround(const MapGround<linearUnit>&rhs) {}

template <int linearUnit> 
MapGround<linearUnit>::MapGround(MapGround<linearUnit>&&rhs) noexcept {}

template <int linearUnit> 
MapGround<linearUnit>& 
MapGround<linearUnit>::operator=(const MapGround<linearUnit>&rhs) {
  return *this;
}

template <int linearUnit> 
MapGround<linearUnit>&MapGround<linearUnit>::operator=(MapGround<linearUnit>&&rhs) noexcept {
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
 *  \brief Given a 3d position in rectification map coordinate system,
 *  output the 3d position in model map coordinate system.
 *
 *  \param [in] rect 3d position in rectification map coordinate system
 *  \return Return 3d position in model map coordinate system
 */
template <int linearUnit> 
Point3d MapGround<linearUnit>::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model map coordinate system,
 *  output the 3d position in rectification map coordinate system.
 *
 *  \param [in] ground 3d position in model map coordinate system
 *  \return Return 3d position in rectification map coordinate system
 */
template <int linearUnit> 
Point3d MapGround<linearUnit>::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
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
