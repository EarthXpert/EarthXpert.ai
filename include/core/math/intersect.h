/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/math/iintersect.h"
#include "core/coretypes.h"
#include "3rdparty/eigen/exeigen.h"

namespace earthxpert {
using thirdparty::IResidual;
using st = SolverTraits;

namespace math {
/**
 *  \brief cost functors implements IResidual
 *
 *  \param PointTy type of point: Point3f, Point3d, Vector3d, etc
 */
 // 3d line equation: p(x, y, z) = p0(x0, y0, z0) + t * v(a, b, c), where v = p1 - p0
template <typename PointTy> struct LineIntersectResidual : IResidual {
  /**
   *  Constructor
   *
   *  \param [in] segs the observation data, containing (numSegs * 2) points.
   *  \param [in] numSegs number of observations
   */
  LineIntersectResidual(const PointTy* segms, const int numSegms) : segs(segms), numSegs(numSegms) {}

  /**
   *  \param [in] pb the two ratios t1 & t2 used to determine the two postions.
   *  \param [out] residual discrepency between the two positions on the two line segments.
   */
  bool operator()(const double* const pb, double* residual) const {
    static_assert(sizeof(double) >= sizeof(PointTy::value_type), "Parameter precision must be not lowert than observation's");
    
    if (2 == numSegs) {
      return XTwoLines(pb, residual);
    }
    else if (3 == numSegs) {
      return XThreeLines(pb, residual);
    }
    else {
      return XMultipleLines(pb, residual);
    }
  }

private:
  const PointTy* segs;  // interleaved by segments each having two end points
  const int numSegs;

  bool XTwoLines(const double* const pb, double* residual) const {
    assert(2 == nunSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto p1 = segs[0] + v1 * pb[0];
    const auto p2 = segs[2] + v2 * pb[1];

    residual[0] = cv::norm(p2 - p1);
    return true;
  }

  bool XThreeLines(const double* const pb, double* residual) const {
    assert(3 == nunSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto v3 = segs[5] - segs[4];
    const auto p1 = segs[0] + v1 * pb[0];
    const auto p2 = segs[2] + v2 * pb[1];
    const auto p3 = segs[4] + v3 * pb[2];

    // point to line distance
    const auto d1Sq = 0;  //dist(p1, segs)
    const auto d2Sq = 0;  //dist(p2, segs + 2)
    const auto d3Sq = 0;  //dist(p3, segs + 4)

    residual[0] = d1Sq + d2Sq + d3Sq;
    return false;
  }

  bool XMultipleLines(const double* const pb, double* residual) const {
    residual[0] = 0.0;

    assert(2 <= nunSegs);
    for (int i = 0; i < numSegs; ++i) {
      const auto v = segs[i * 2 + 1] - segs[i * 2];
      const auto p = segs[i * 2] + v * pb[i];
      const auto dSq = 0; //dist(p, segs + i*2)
      residual[0] += dSq;
    }
    return false;
  }
};

/**
 *  \brief cost functors implements IResidual
 *
 *  \param SphereType type of sphere: Circle3f, Circle3d, etc
 */
 // 3d sphere equation: r = distance(p(x, y, z), p0(x0, y0, z0))
template <typename SphereType> struct SphereIntersectResidual {
};

/**
 *  \brief find intersection of multiple 3D line segments (optical) or sphere surface (gps, radar).
 *
 *  \param GeomTy type of geometry: Point3f, Point3d, Vector3d for line segments;
 *  CircleF, CircleD for sphere.
 *  \param ResidualPolicy defines the functor for residual calculation.
 *
 *  \param [in] geoms a sequence, at least two geometries. e.g., 
 *  each line segment is defined by two end points; each sphere by the center point and the radius.
 *  \param [in] numGeoms number of geometris to intersect.
 *  \param [in/out] pb the parameter block holding all unknown parameters
 *                  optionally with initial values.
 *  \param [in] options the options to control ceres solver.
 *  \param [out] summary the report of ceres solver.
 *
 *  \todo do not instantiate LossPolicy before resolving the construtor issue
 *  \todo estimate initial parameter values
 *  \todo weighting of observations
 *  \todo make CostFunction a template parameter
 */
template <typename GeomTy,
  template <typename GTy = GeomTy> class ResidualPolicy, class LossPolicy = void>
bool IntersectGeometries(const GeomTy* geoms, const int numGeoms, std::vector<double>& pb,
    const Solver::Options& options, Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  static_assert(std::is_base_of_v<IResidual, ResidualPolicy<GeomTy>>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(geoms && 0 < numGeoms);

  if (pb.empty()) {
    pb.resize(numGeoms, 0.5);
  }

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;

  Problem problem;
  auto costFunc = new AutoDiffCostFunction<ResidualPolicy<GeomTy>, 1, 2>(
    new ResidualPolicy<GeomTy>(geoms, numGeoms));

  problem.AddResidualBlock(costFunc, lossF, pb.data()); // numGeoms

  ceres::Solve(options, &problem, &summary);
  return summary.IsSolutionUsable();
}

/**
 *  The base class Intersect implements the template methods for the intersection of 3D geometries.
 *
 *  \todo refactor into ceres and Eigen solvers
 */
class EXCORE_EXPORTS Intersect : public IIntersect {
public:
  Intersect();
  virtual ~Intersect() = default;

  void SetOptions(const Solver::Options& options) { mOptions = options; }

  auto& GetOptions() noexcept { return mOptions; }

protected:
  Solver::Options mOptions;   // the options to control ceres solver
};

/**
 *  \brief The class XTwoLines finds the intersection of two 3D line segments.
 */
//template <int solverType = kST_EIGEN>
class EXCORE_EXPORTS XTwoLines final : public Intersect {
public:
  static constexpr size_t kSolverCapability = st::kBITCERES & st::kBITEIGEN & st::kBITEXAI;
  static constexpr int kNumRays = 2;

  ~XTwoLines() = default;

  /**
   *  Intersect two 3D line segements (ceres).
   *
   *  \param [in] segs the list of four end points interlaeved by line segments, 
   *  each is defined by two points.
   *  \param [out] p the intersection point.
   *  \param [out] summary the report of ceres solver.
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, 
    Solver::Summary& summary, int numSegs = 2) const override;

  /**
   *  Intersect two 3D line segements (Eigen).
   *
   *  \param [in] segs the list of four end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2
   *  \return Return true if successful, false if parallel.
   */
  inline bool Intersect(const Point3d* segs, Point3d& p, 
    const int numSegs = 2) const override {
    return Intersect(segs, segs + 2, p);
  }

  /**
   *  Intersect two 3D line segements (Eigen).
   *
   *  \param [in] s1 the first line segment defined by two end points
   *  \param [in] s2 the second line segment defined by two end points
   *  \param [out] p the intersection point.
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* s1, const Point3d* s2, Point3d& p) const;
};

//using XTwoLinesEigen = XTwoLines<kST_EIGEN>;

/**
 *  \brief The class XThreeLines finds the intersection of three 3D line segments.
 */
class EXCORE_EXPORTS XThreeLines final : public Intersect {
public:
  static constexpr size_t kSolverCapability = st::kBITCERES & st::kBITEIGEN;
  static constexpr int kNumRays = 3;

  ~XThreeLines() = default;

  /**
   *  Intersect three 3D line segements (ceres).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, 
   *  each is defined by two points.
   *  \param [out] p the intersection point.
   *  \param [out] summary the report of ceres solver.
   *  \param [in] numSegs number of line segments given by segs, must be 3
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, 
    Solver::Summary& summary, int numSegs = 3) const override;

  /**
   *  Intersect three 3D line segements (Eigen).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 3
   *  \return Return true if successful, false if parallel.
   */
  inline bool Intersect(const Point3d * segs, Point3d & p, const int numSegs = 3) const override {
    return Intersect(segs, segs + 2, segs + 4, p);
  }

  /**
   *  Intersect three 3D line segements (Eigen).
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
class EXCORE_EXPORTS XMultipleLines final : public Intersect{
public:
  static constexpr size_t kSolverCapability = st::kBITCERES & st::kBITEIGEN;
  static constexpr int kNumRays = -1;   // dynamic

  ~XMultipleLines() = default;

  /**
   *  Intersect multiple 3D line segements (ceres).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2+
   *  \param [out] summary the report of ceres solver.
   *  \param [in] numSegs number of line segments given by segs, must be 2+
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, 
    Solver::Summary& summary, const int numSegs) const override {
    assert(2 <= numSegs);
    return true;
  }

  /**
   *  Intersect multiple 3D line segements (Eigen).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2+
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, const int numSegs) const override {
    assert(2 <= numSegs);
    return true;
  }
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
}
} // namespace earthxpert
