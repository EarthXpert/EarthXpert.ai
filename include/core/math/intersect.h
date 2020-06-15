/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/math/iintersect.h"

#include "3rdparty/ceres/exceres.h"
#include "3rdparty/eigen/exeigen.h"

namespace earthxpert::math {
/**
 *  \brief The class XTwoLines finds the intersection of two 3D line segments.
 */
class EXCORE_EXPORTS XTwoLines final : public IIntersect {
public:
  static constexpr int kNumRays = 2;

  ~XTwoLines() = default;

  /**
   *  Intersect two 3D line segements.
   *
   *  \param [in] segs the list of four end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, const int numSegs = 2) const override {
    return Intersect(segs, segs + 2, p);
  }

  /**
   *  Intersect two 3D line segements.
   *
   *  \param [in] s1 the first line segment defined by two end points
   *  \param [in] s2 the second line segment defined by two end points
   *  \param [out] p the intersection point.
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* s1, const Point3d* s2, Point3d& p) const;
};

/**
 *  \brief The class XThreeLines finds the intersection of three 3D line segments.
 */
class EXCORE_EXPORTS XThreeLines final : public IIntersect {
public:
  static constexpr int kNumRays = 3;

  ~XThreeLines() = default;

  /**
   *  Intersect three 3D line segements.
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 3
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d * segs, Point3d & p, const int numSegs = 3) const override {
    return Intersect(segs, segs + 2, segs + 4, p);
  }

  /**
   *  Intersect three 3D line segements.
   *
   *  \param [in] s1 the first line segment defined by two end points
   *  \param [in] s2 the second line segment defined by two end points
   *  \param [in] s3 the second line segment defined by two end points
   *  \param [out] p the intersection point.
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* s1, const Point3d* s2, const Point3d* s3, Point3d& p) const;
};

/**
 *  \brief The class XMultipleLines finds the intersection of two or more 3D line segments.
 */
class EXCORE_EXPORTS XMultipleLines final : public IIntersect{
public:
  static constexpr int kNumRays = -1;   // dynamic

  ~XMultipleLines() = default;

  /**
   *  Intersect three 3D line segements.
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2+
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d * segs, Point3d & p, const int numSegs) const override {
    assert(2 <= numSegs);
    return true;
  }

private:
};

// helper free functions
/**
 * Intersect two 3D line segements.
 *
 *  \return Return true if successful, false if parallel.
 */
inline EXCORE_EXPORTS bool IntersectTwoLines(const Point3d* s1, const Point3d* s2, Point3d& p) {
  XTwoLines xl;
  return xl.Intersect(s1, s2, p);
}

/**
 * Intersect three 3D line segements.
 *
 *  \return Return true if successful, false if parallel.
 */
inline EXCORE_EXPORTS bool IntersectThreeLines(const Point3d* s1, const Point3d* s2,
  const Point3d* s3, Point3d& p) {
  XThreeLines xl;
  return xl.Intersect(s1, s2, s3, p);
}

/**
 * Intersect two or more 3D line segements.
 *
 *  \return Return true if successful, false if parallel.
 */
inline EXCORE_EXPORTS bool IntersectMultipleLines(const Point3d* segs, 
  Point3d& p, const int numSegs) {
  XMultipleLines xl;
  return xl.Intersect(segs, p, numSegs);
}
} // namespace earthxpert::math
