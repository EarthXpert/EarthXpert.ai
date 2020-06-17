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
// residual base for intersection
template <typename GeomTy> struct IntersectResidual : IResidual {
  IntersectResidual(const GeomTy* segms, const int numSegms) : segs(segms), numSegs(numSegms) {}
  virtual ~IntersectResidual() = default;

protected:
  const GeomTy* segs;  // pointer to an array of geometries
  const int numSegs;
};

/**
 *  \brief AutoDiff cost functors implements IResidual
 *
 *  \param PointTy type of point: Point3f, Point3d, Vector3d, etc
 */
 // 3d line equation: p(x, y, z) = p0(x0, y0, z0) + t * v(a, b, c), where v = p1 - p0
template <typename PointTy> struct AutoLineIntersectResidual final : IntersectResidual<PointTy> {
  /**
   *  \param [in] segs the packed observation data, containing (numSegs * 2) points.
   *  \param [in] numSegs number of observations
   */
  AutoLineIntersectResidual(const PointTy* segms, const int numSegms) 
    : IntersectResidual<PointTy>(segms, numSegms) {}
  ~AutoLineIntersectResidual() = default;

  /**
   *  \brief Fixed functor
   *
   *  \param [in] pb the ratios {t1, t2, ...} used to determine the postions.
   *  \param [out] residual discrepency between the two positions on the two line segments.
   */
  template <typename Ty = double> bool operator()(const Ty* const pb, Ty* residual) const {
    static_assert(sizeof(Ty) >= sizeof(PointTy::value_type), "Parameter precision must be not lowert than observation's");

    if (2 == numSegs) {
      return TwoLines(pb, residual);
    }
    else if (3 == numSegs) {
      return ThreeLines(pb, residual);
    }
    else {
      return MultipleLines(pb, residual);
    }
  }

  // Dynamic functor
  bool operator()(double const* const* pb, double* residual) const {
    if (2 == numSegs) {
      return TwoLines(pb, residual);
    }
    else if (3 == numSegs) {
      return ThreeLines(pb, residual);
    }
    else {
      return MultipleLines(pb, residual);
    }
    return true;
  }

private:
  /**
   *  \issue AutoDiff compiling error: binary '*': no operator found which takes a right-hand operand of type 'const JetT'
   *  \remark 3x faster than calculating the L2 norm: residual[0] = cv::norm(p2 - p1);
   */
  template <typename Ty> bool TwoLines(const Ty* const pb, Ty* residual) const {
    assert(2 == numSegs);
    const Point3d v1 = segs[1] - segs[0];
    const Point3d v2 = segs[3] - segs[2];
    const Point3d p1 = segs[0] + v1 * pb[0];
    const Point3d p2 = segs[2] + v2 * pb[1];

    residual[0] = p2.x - p1.x;
    residual[1] = p2.y - p1.y;
    residual[2] = p2.z - p1.z;
    return true;
  }

  /**
   *  \issue AutoDiff compiling error: cannot convert from 'ceres::Jet<double,2>' to 'double'
  template <typename Ty> bool TwoLines(const Ty* const pb, Ty* residual) const {
    assert(2 == numSegs);
    Point3d v1; // segs[1] - segs[0]
    v1.x = segs[1].x - segs[0].x;
    v1.y = segs[1].y - segs[0].y;
    v1.z = segs[1].z - segs[0].z;

    Point3d v2; // segs[3] - segs[2]
    v2.x = segs[3].x - segs[2].x;
    v2.y = segs[3].y - segs[2].y;
    v2.z = segs[3].z - segs[2].z;

    Point3d p1; // segs[0] + v1 * pb[0]
    p1.x = segs[0].x + v1.x * pb[0];
    p1.y = segs[0].y + v1.y * pb[0];
    p1.z = segs[0].z + v1.z * pb[0];

    Point3d p2; // segs[2] + v2 * pb[1]
    p2.x = segs[2].x + v2.x * pb[1];
    p2.y = segs[2].y + v2.y * pb[1];
    p2.z = segs[2].z + v2.z * pb[1];

    residual[0] = cv::norm(p2 - p1);
    return true;
  }
   */

  bool TwoLines(double const* const* pb, double* residual) const {
    assert(2 == numSegs);
    const Point3d v1 = segs[1] - segs[0];
    const Point3d v2 = segs[3] - segs[2];
    const Point3d p1 = segs[0] + v1 * (*pb[0]);
    const Point3d p2 = segs[2] + v2 * (*pb[1]);

    residual[0] = p2.x - p1.x;
    residual[1] = p2.y - p1.y;
    residual[2] = p2.z - p1.z;
    return true;
  }

  template <typename Ty> bool ThreeLines(const Ty* const pb, Ty* residual) const {
    assert(3 == numSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto v3 = segs[5] - segs[4];
    const auto p1 = segs[0] + v1 * pb[0]; // t1
    const auto p2 = segs[2] + v2 * pb[1]; // t2
    const auto p3 = segs[4] + v3 * pb[2]; // t3

    // point to line distance
    const auto d1Sq = 0;  //dist(p1, segs)
    const auto d2Sq = 0;  //dist(p2, segs + 2)
    const auto d3Sq = 0;  //dist(p3, segs + 4)

    residual[0] = d1Sq + d2Sq + d3Sq;
    return true;
  }

  bool ThreeLines(double const* const* pb, double* residual) const {
    assert(3 == numSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto v3 = segs[5] - segs[4];
    const auto p1 = segs[0] + v1 * (*pb[0]); // t1
    const auto p2 = segs[2] + v2 * (*pb[1]); // t2
    const auto p3 = segs[4] + v3 * (*pb[2]); // t3

    // point to line distance
    const auto d1Sq = 0;  //dist(p1, segs)
    const auto d2Sq = 0;  //dist(p2, segs + 2)
    const auto d3Sq = 0;  //dist(p3, segs + 4)

    residual[0] = d1Sq + d2Sq + d3Sq;
    return true;
  }

  template <typename Ty> bool MultipleLines(const Ty* const pb, Ty* residual) const {
    assert(2 <= numSegs);
    residual[0] = Ty(0.0);

    for (int i = 0; i < numSegs; ++i) {
      const auto v = segs[i * 2 + 1] - segs[i * 2];
      const auto p = segs[i * 2] + v * pb[i]; // t
      const auto dSq = 0; //dist(p, segs + i*2)

      residual[0] += dSq;
    }
    return true;
  }

  bool MultipleLines(double const* const* pb, double* residual) const {
    assert(2 <= numSegs);
    residual[0] = 0.0;

    for (int i = 0; i < numSegs; ++i) {
      const auto v = segs[i * 2 + 1] - segs[i * 2];
      const auto p = segs[i * 2] + v * (*pb[i]); // t
      const auto dSq = 0; //dist(p, segs + i*2)

      residual[0] += dSq;
    }
    return true;
  }
};

/**
 *  \brief NumericDiff cost functors implements IResidual
 *
 *  \param PointTy type of point: Point3f, Point3d, Vector3d, etc
 */
 // 3d line equation: p(x, y, z) = p0(x0, y0, z0) + t * v(a, b, c), where v = p1 - p0
template <typename PointTy> struct NumericLineIntersectResidual final : IntersectResidual<PointTy> {
  /**
   *  \param [in] segs the packed observation data, containing (numSegs * 2) points.
   *  \param [in] numSegs number of observations
   */
  NumericLineIntersectResidual(const PointTy* segms, const int numSegms) 
    : IntersectResidual<PointTy>(segms, numSegms) {}
  ~NumericLineIntersectResidual() = default;

  /**
   *  \brief Fixed functor
   *
   *  \param [in] pb the ratios {t1, t2, ...} used to determine the postions.
   *  \param [out] residual discrepency between the two positions on the two line segments.
   */
  template <typename Ty = double> bool operator()(const Ty* const pb, Ty* residual) const {
    static_assert(sizeof(Ty) >= sizeof(PointTy::value_type), "Parameter precision must be not lowert than observation's");
    
    if (2 == numSegs) {
      return TwoLines(pb, residual);
    }
    else if (3 == numSegs) {
      return ThreeLines(pb, residual);
    }
    else {
      return MultipleLines(pb, residual);
    }
  }

  // Dynamic functor
  bool operator()(double const* const* pb, double* residual) const {
    if (2 == numSegs) {
      return TwoLines(pb, residual);
    }
    else if (3 == numSegs) {
      return ThreeLines(pb, residual);
    }
    else {
      return MultipleLines(pb, residual);
    }
    return true;
  }

private:
  /**
   *  \remark 3x faster than calculating the L2 norm: residual[0] = cv::norm(p2 - p1);
   */
  template <typename Ty> bool TwoLines(const Ty* const pb, Ty* residual) const {
    assert(2 == numSegs);
    const Point3d v1 = segs[1] - segs[0];
    const Point3d v2 = segs[3] - segs[2];
    const Point3d p1 = segs[0] + v1 * pb[0];
    const Point3d p2 = segs[2] + v2 * pb[1];

    residual[0] = p2.x - p1.x;
    residual[1] = p2.y - p1.y;
    residual[2] = p2.z - p1.z;
    return true;
  }

  bool TwoLines(double const* const* pb, double* residual) const {
    assert(2 == numSegs);
    const Point3d v1 = segs[1] - segs[0];
    const Point3d v2 = segs[3] - segs[2];
    const Point3d p1 = segs[0] + v1 * (*pb[0]);
    const Point3d p2 = segs[2] + v2 * (*pb[1]);

    residual[0] = p2.x - p1.x;
    residual[1] = p2.y - p1.y;
    residual[2] = p2.z - p1.z;
    return true;
  }

  template <typename Ty> bool ThreeLines(const Ty* const pb, Ty* residual) const {
    assert(3 == numSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto v3 = segs[5] - segs[4];
    const auto p1 = segs[0] + v1 * pb[0]; // t1
    const auto p2 = segs[2] + v2 * pb[1]; // t2
    const auto p3 = segs[4] + v3 * pb[2]; // t3

    // point to line distance
    const auto d1Sq = 0;  //dist(p1, segs)
    const auto d2Sq = 0;  //dist(p2, segs + 2)
    const auto d3Sq = 0;  //dist(p3, segs + 4)

    residual[0] = d1Sq + d2Sq + d3Sq;
    return true;
  }

  bool ThreeLines(double const* const* pb, double* residual) const {
    assert(3 == numSegs);
    const auto v1 = segs[1] - segs[0];
    const auto v2 = segs[3] - segs[2];
    const auto v3 = segs[5] - segs[4];
    const auto p1 = segs[0] + v1 * (*pb[0]); // t1
    const auto p2 = segs[2] + v2 * (*pb[1]); // t2
    const auto p3 = segs[4] + v3 * (*pb[2]); // t3

    // point to line distance
    const auto d1Sq = 0;  //dist(p1, segs)
    const auto d2Sq = 0;  //dist(p2, segs + 2)
    const auto d3Sq = 0;  //dist(p3, segs + 4)

    residual[0] = d1Sq + d2Sq + d3Sq;
    return true;
  }

  template <typename Ty> bool MultipleLines(const Ty* const pb, Ty* residual) const {
    assert(2 <= numSegs);
    residual[0] = Ty(0.0);

    for (int i = 0; i < numSegs; ++i) {
      const auto v = segs[i * 2 + 1] - segs[i * 2];
      const auto p = segs[i * 2] + v * pb[i]; // t
      const auto dSq = 0; //dist(p, segs + i*2)

      residual[0] += dSq;
    }
    return true;
  }

  bool MultipleLines(double const* const* pb, double* residual) const {
    assert(2 <= numSegs);
    residual[0] = 0.0;

    for (int i = 0; i < numSegs; ++i) {
      const auto v = segs[i * 2 + 1] - segs[i * 2];
      const auto p = segs[i * 2] + v * (*pb[i]); // t
      const auto dSq = 0; //dist(p, segs + i*2)

      residual[0] += dSq;
    }
    return true;
  }
};

/**
 *  \brief AutoDiff cost functors implements IResidual
 *
 *  \param SphereType type of sphere: Circle3f, Circle3d, etc
 */
 // 3d sphere equation: r = distance(p(x, y, z), p0(x0, y0, z0))
template <typename SphereType> struct AutoSphereIntersectResidual final : IntersectResidual<SphereType> {
  AutoSphereIntersectResidual(const SphereType* sphs, const int numSphs) : IntersectResidual<SphereType>(sphs, numSphs) {}
  ~AutoSphereIntersectResidual() = default;
};

/**
 *  \brief NumericDiff cost functors implements IResidual
 *
 *  \param SphereType type of sphere: Circle3f, Circle3d, etc
 */
 // 3d sphere equation: r = distance(p(x, y, z), p0(x0, y0, z0))
template <typename SphereType> struct NumericSphereIntersectResidual final : IntersectResidual<SphereType> {
  NumericSphereIntersectResidual(const SphereType* sphs, const int numSphs) : IntersectResidual<SphereType>(sphs, numSphs) {}
  ~NumericSphereIntersectResidual() = default;
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
 *  \todo make CostFunction a template parameter: AutoDiff, NumericDiff
 *  \todo variadic Ns: specify size of parameter blocks at run-time
 */
template <typename GeomTy,
  template <typename GTy = GeomTy> class ResidualPolicy, class LossPolicy = void>
bool IntersectGeometries(const GeomTy* geoms, const int numGeoms, std::vector<double>& pb,
    const Solver::Options& options, Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  static_assert(std::is_base_of_v<IResidual, ResidualPolicy<GeomTy>>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(geoms && 2 <= numGeoms);

  if (pb.empty()) {
    pb.resize(numGeoms, 0.5); // initial middle point
  }

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;
  Problem problem;

  // Dynamic parameter blocks
  //auto costFunc = new DynamicAutoDiffCostFunction<ResidualPolicy<GeomTy>>(
  //  new ResidualPolicy<GeomTy>(geoms, numGeoms));
  //costFunc->SetNumResiduals(3);
  //costFunc->AddParameterBlock(numGeoms);

  //auto costFunc = new DynamicNumericDiffCostFunction<ResidualPolicy<GeomTy>, ceres::CENTRAL>(
  //  new ResidualPolicy<GeomTy>(geoms, numGeoms));
  //costFunc->SetNumResiduals(3);
  //costFunc->AddParameterBlock(numGeoms);

  // Sized parameter blocks
  //auto costFunc = new AutoDiffCostFunction<ResidualPolicy<GeomTy>, 3, 2>(
  //  new ResidualPolicy<GeomTy>(geoms, numGeoms));
  
  auto costFunc = new NumericDiffCostFunction<ResidualPolicy<GeomTy>, ceres::CENTRAL, 3, 2>(
    new ResidualPolicy<GeomTy>(geoms, numGeoms));

  problem.AddResidualBlock(costFunc, lossF, pb.data()); 

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
  static constexpr int kNumLines = 2;

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
    Solver::Summary& summary, int = 2) const override;

  /**
   *  Intersect two 3D line segements (Eigen).
   *
   *  \param [in] segs the list of four end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2
   *  \return Return true if successful, false if parallel.
   */
  inline bool Intersect(const Point3d* segs, Point3d& p, const int = 2) const override {
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
  static constexpr int kNumLines = 3;

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
    Solver::Summary& summary, int = 3) const override;

  /**
   *  Intersect three 3D line segements (Eigen).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 3
   *  \return Return true if successful, false if parallel.
   */
  inline bool Intersect(const Point3d * segs, Point3d & p, const int = 3) const override {
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
  static constexpr int kNumLines = -1;   // dynamic

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
    Solver::Summary& summary, const int numSegs) const override;

  /**
   *  Intersect multiple 3D line segements (Eigen).
   *
   *  \param [in] segs the list of six end points interlaeved by line segments, each is
   *  defined by two points.
   *  \param [out] p the intersection point.
   *  \param [in] numSegs number of line segments given by segs, must be 2+
   *  \return Return true if successful, false if parallel.
   */
  bool Intersect(const Point3d* segs, Point3d& p, const int numSegs) const override;
};

// helper free functions
/**
 * Intersect two 3D line segements (ceres).
 *
 *  \return Return true if successful, false if parallel.
 */
inline EXCORE_EXPORTS bool IntersectTwoLines(const Point3d* segs, 
  Point3d& p, Solver::Summary& summary) {
  XTwoLines xl;
  return xl.Intersect(segs, p, summary);
}

/**
 * Intersect two 3D line segements (Eigen).
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
