/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/ceres/exceres.h"
#include "3rdparty/opencv/exopencv.h"

#include <utility>

using ceres::Solver;

namespace earthxpert::math {
/**
 *  The class GraphFit computes the parameters of various graphical line,
 *  curve, plane, surface, high-dimensional super monifolds that are
 *  represented by functions like y = f(x), where x is N-dimentional
 *  input variables, and y is the scalar result, totaling an N+1
 *  -dimensional observation vector of (x, y).
 */
class EXCORE_EXPORTS GraphFit {
public:
  // Library used to solve algebraic problems
  enum SolverType {
    eCERES,
    eEIGEN
  };

  GraphFit();
  GraphFit(const Solver::Options& options);
  ~GraphFit() = default;

  void SetOptions(const Solver::Options& options) { mOptions = options; }

  auto& GetOptions() { return mOptions; }

  /**
   *  \breif fit a parameterized stright line to a sequence of 2D points.
   *  The line equation is defined by
   *
   *    y = m * x + c
   *
   *  where
   *
   *    y: output variable
   *    x: input variable
   *    m: the slope parameter
   *    c: the intercept parameter
   *
   *  \param [in] points a sequence, at least two, of 2D points
   *  \param [in/out] pb the parameter block holding all unknown parameters
   *                  optionally with initial values.
   *  \param [out] summary the report of ceres solver.
   *
   *  \todo vertical line: x = constant
   */
  bool FitLineCeres(const std::vector<Point2d>& points, 
    std::vector<double>& pb, Solver::Summary& summary);
  bool FitLineCeresRobust(const std::vector<Point2d>& points, 
    std::vector<double>& pb, Solver::Summary& summary);

  bool FitLineCv(const std::vector<Point2d>& points, cv::Vec2d& pb);
  bool FitLineCv(const std::vector<Point2d>& points, std::array<double, 2>& pb);

  bool FitLineEigen(const std::vector<Point2d>& points, std::vector<double>& pb);

  /**
   *  \breif fit a parameterized stright line to a sequence of 3D points.
   *  The line equation is defined by
   *
   *    p = p0 + v * t
   *
   *  where
   *
   *    p: the observed 3D point
   *    p0: the anchor point, can be initialized by the average position or a selected observation
   *    v: the direction vector
   *    t: the parametric ratio
   *
   *  \param [in] points a sequence, at least two, of 3D points
   *  \param [in/out] pb the parameter block holding all unknown parameters
   *                  optionally with initial values.
   *  \param [out] summary the report of ceres solver.
   */
  bool FitLineCeres(const std::vector<Point3d>& points,
    std::vector<double>& pb, Solver::Summary& summary);

  bool FitLineCv(const std::vector<Point3d>& points, cv::Vec6d& pb);

  bool FitLineEigen(const std::vector<Point3d>& points, std::vector<double>& pb);

  /**
   *  \breif fit a parameterized exponential curve to a sequence of 2D points.
   *  The line equation is defined by
   *
   *    y = exp(m * x + c)
   *
   *  where
   *
   *    y: output variable
   *    x: input variable
   *    m: the slope parameter
   *    c: the intercept parameter
   *
   *  \param [in] points a sequence, at least two, of 2D points
   *  \param [in/out] pb the parameter block holding all unknown parameters
   *                  optionally with initial values.
   *  \param [out] summary the report of ceres solver.
   */
  bool FitCurveCeres(const std::vector<Point2d>& points,
    std::vector<double>& pb, Solver::Summary& summary);

  /**
   *  \breif fit a parameterized plane to a sequence of 3D points.
   *  The plane equation is defined by
   *
   *    z = m * x + n * y + c
   *
   *  where
   *
   *    z: output variable
   *    x,y: input variables
   *    m,n: the slope parameters
   *    c: the intercept parameter
   *
   *  \param [in] points a sequence, at least three non-colinear, of 3D points
   *  \param [in/out] pb the parameter block holding all unknown parameters
   *                  optionally with initial values.
   *  \param [out] summary the report of ceres solver.
   */
  bool FitPlaneCeres(const std::vector<Point3d>& points,
    std::vector<double>& pb, Solver::Summary& summary);

  bool FitPlaneEigen(const std::vector<Point3d>& points, std::vector<double>& pb);

private:
  Solver::Options mOptions;   // the options to control ceres solver
  //Solver::Summary mSummary; // the report of ceres solver
};
} // namespace earthxpert::math
