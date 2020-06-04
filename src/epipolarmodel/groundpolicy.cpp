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
template <int LinearUnit> LocalGround<LinearUnit>::LocalGround() {
}

template <int LinearUnit> 
LocalGround<LinearUnit>::LocalGround(const LocalGround<LinearUnit>&rhs) {}

template <int LinearUnit> 
LocalGround<LinearUnit>::LocalGround(LocalGround<LinearUnit>&&rhs) noexcept {}

template <int LinearUnit> LocalGround<LinearUnit>& 
LocalGround<LinearUnit>::operator=(const LocalGround<LinearUnit>&rhs) {
  return *this;
}

template <int LinearUnit> 
LocalGround<LinearUnit>& LocalGround<LinearUnit>::operator=(LocalGround<LinearUnit>&&rhs) noexcept {
  return *this;
}

template <int LinearUnit> 
bool LocalGround<LinearUnit>::operator==(const LocalGround<LinearUnit>&rhs) const {
  return true;
}

template <int LinearUnit> 
bool LocalGround<LinearUnit>::operator!=(const LocalGround<LinearUnit>&rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification LSR coordinate system,
 *  output the 3d position in model LSR coordinate system.
 *
 *  \param [in] rect 3d position in rectification LSR coordinate system
 *  \return Return 3d position in model LSR coordinate system
 */
template <int LinearUnit> 
Point3d LocalGround<LinearUnit>::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model LSR coordinate system,
 *  output the 3d position in rectification LSR coordinate system.
 *
 *  \param [in] ground 3d position in model LSR coordinate system
 *  \return Return 3d position in rectification LSR coordinate system
 */
template <int LinearUnit> 
Point3d LocalGround<LinearUnit>::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <int LinearUnit> 
void LocalGround<LinearUnit>::CopyFrom(const LocalGround &rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
template <int LinearUnit> 
void LocalGround<LinearUnit>::MoveFrom(LocalGround &&rhs) noexcept {
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
template <int LinearUnit> MapGround<LinearUnit>::MapGround() {
}

template <int LinearUnit> 
MapGround<LinearUnit>::MapGround(const MapGround<LinearUnit>&rhs) {}

template <int LinearUnit> 
MapGround<LinearUnit>::MapGround(MapGround<LinearUnit>&&rhs) noexcept {}

template <int LinearUnit> 
MapGround<LinearUnit>& 
MapGround<LinearUnit>::operator=(const MapGround<LinearUnit>&rhs) {
  return *this;
}

template <int LinearUnit> 
MapGround<LinearUnit>&MapGround<LinearUnit>::operator=(MapGround<LinearUnit>&&rhs) noexcept {
  return *this;
}

template <int LinearUnit> 
bool MapGround<LinearUnit>::operator==(const MapGround<LinearUnit>&rhs) const {
  return true;
}

template <int LinearUnit> 
bool MapGround<LinearUnit>::operator!=(const MapGround<LinearUnit>&rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Given a 3d position in rectification map coordinate system,
 *  output the 3d position in model map coordinate system.
 *
 *  \param [in] rect 3d position in rectification map coordinate system
 *  \return Return 3d position in model map coordinate system
 */
template <int LinearUnit> 
Point3d MapGround<LinearUnit>::RectToGround(const Point3d &rect) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief Given a 3d position in model map coordinate system,
 *  output the 3d position in rectification map coordinate system.
 *
 *  \param [in] ground 3d position in model map coordinate system
 *  \return Return 3d position in rectification map coordinate system
 */
template <int LinearUnit> 
Point3d MapGround<LinearUnit>::GroundToRect(const Point3d &ground) const {
  return Point3d(0, 0, 0);
}

/**
 *  \brief helper of copy constructor & assignment.
 */
template <int LinearUnit> 
void MapGround<LinearUnit>::CopyFrom(const MapGround<LinearUnit>&rhs) {
}

/**
 *  \brief helper of move constructor & assignment.
 */
template <int LinearUnit> 
void MapGround<LinearUnit>::MoveFrom(MapGround<LinearUnit>&&rhs) noexcept  {
}

template class earthxpert::MapGround<earthxpert::kLU_FEET>;
template class earthxpert::MapGround<earthxpert::kLU_METER>;
template class earthxpert::MapGround<earthxpert::kLU_USFEET>;
