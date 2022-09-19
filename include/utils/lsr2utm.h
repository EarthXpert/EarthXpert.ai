/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "utils/exutils.h"
#include "utils/lsr2utmct.h"

#include <memory>

using ceres::Solver;

namespace earthxpert {
using thirdparty::IResidual;

/**
 *  \brief NumericDiff cost functors implements IResidual
 */
struct NumericLsr2UtmResidual final : IResidual {
  /**
   *  \param [in] lsr.
   *  \param [in] utm
   *  \param [in] ct
   */
  NumericLsr2UtmResidual() = default;
  NumericLsr2UtmResidual(const Point3d* const lsr, const Point2d* const utm, const LsrToUtmCT* const ct)
    : lsr(lsr), utm(utm), ct(ct) {}
  ~NumericLsr2UtmResidual() = default;

  void SetArgs(const Point3d* const lsr, const Point2d* const utm, const LsrToUtmCT* const ct) {
    this->lsr = lsr;
    this->utm = utm;
    this->ct = ct;
  }

  bool operator()(const double* const pb, double* residual) const {
    const auto utmT = ct->Convert(lsr, pb);

    residual[0] = utmT.x - utm->x;
    residual[1] = utmT.y - utm->y;
    return true;
  }

private:
  const Point3d* lsr{};
  const Point2d* utm{};
  const LsrToUtmCT* ct{};
};

/**
 *  The class LsrToUtm converts points in LSR to be in UTM.
 */
class EXUTILS_EXPORTS LsrToUtm {
public:
  LsrToUtm(const Point3d& origin);
  ~LsrToUtm() = default;

  bool Solve(const std::vector<Point3d>& lsrs, const std::vector<Point2d>& utms, double* pb);

  std::vector<Point2d> Convert(const std::vector<Point3d>& lsrs, const double* pb) const;

private:
  std::unique_ptr<LsrToUtmCT> mCT;

  Solver::Options mOptions;   // the options to control ceres solver

  void Initialize();
  void SetParameterBounds(ceres::Problem& problem, double* pb);
};

using UPLsrToUtm = std::unique_ptr<LsrToUtm>;
using SPLsrToUtm = std::shared_ptr<LsrToUtm>;
} // namespace earthxpert
