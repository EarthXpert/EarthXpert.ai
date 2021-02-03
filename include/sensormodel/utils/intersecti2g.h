/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "3rdparty/ceres/exceres.h"
#include "3rdparty/opencv/exopencv.h"
#include "core/coretypes.h"

using ceres::Solver;

namespace earthxpert {
using thirdparty::IResidual;

/**
 *  \brief AnalyticDiff cost functors implements CostFunction
 */
class AnalyticG2ICostFunc final : ceres::DynamicCostFunction {

};

/**
 *  \brief AutoDiff cost functors implements IResidual
 *
 *  \remark requires significant changes to SensorModel
 */
struct AutoG2IResidual final : IResidual {

};

/**
 *  \brief NumericDiff cost functors implements IResidual
 */
struct NumericG2IResidual final : IResidual {
  /**
   *  \param [in] sensorModel.
   *  \param [in] image
   */
  NumericG2IResidual() {}
  NumericG2IResidual(const Point2d* const image, const ISensorModel* const sensorModel)
    : image(image), sensorModel(sensorModel) {}
  ~NumericG2IResidual() = default;

  void SetArgs(const Point2d* const image, const ISensorModel* const sensorModel) {
    this->image = image;
    this->sensorModel = sensorModel;
  }

  bool operator()(const double* const pb, double* residual) const {
    //Point3d ground(pb[0], pb[1], pb[2]);
    auto ground = reinterpret_cast<const Point3d* const>(pb);
    Point2d ip;
    const auto ret = sensorModel->GroundToImage(*ground, ip);
    if (!ret) return false;

    residual[0] = ip.x - image->x;
    residual[1] = ip.y - image->y;
    return true;
  }

private:
  const Point2d* image;
  const ISensorModel* sensorModel;
};

/**
 *  \brief find intersection of multiple 3D rays (optical, radar), each is
 *  represented by an image point and associated senosr model.
 */
template <class ResidualPolicy, class LossPolicy = void>
bool IntersectI2G0(const Point2d* images, const ISensorModel* const* sensorModels,
  const int num, Point3d& ground, const Solver::Options& options,
  Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  using CostFunctor = ResidualPolicy;
  static_assert(std::is_base_of_v<IResidual, CostFunctor>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(images && sensorModels && 2 <= num);

  Problem::Options popt;
  popt.loss_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  Problem problem(popt);

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;
  
  for (int i = 0; i < num; ++i) {
    auto costFunc = new NumericDiffCostFunction<CostFunctor, ceres::CENTRAL, 2, 3>(
      new CostFunctor(images + i, sensorModels[i]));

    problem.AddResidualBlock(costFunc, lossF, reinterpret_cast<double*>(&ground));
  }
  ceres::Solve(options, &problem, &summary);

  return summary.IsSolutionUsable();
}

// \todo pre-allocate CostFunction & CostFunctor 
// \todo round up sizeof(NumericDiffCostFunc) to a multiplier of alignof(NumericDiffCostFunc)
template <class ResidualPolicy, class LossPolicy = void>
bool IntersectI2G(const Point2d* images, const ISensorModel* const* sensorModels,
  const int num, Point3d& ground, const Solver::Options& options,
  Solver::Summary& summary, LossPolicy* lossFunc = nullptr) {
  using CostFunctor = ResidualPolicy;
  static_assert(std::is_base_of_v<IResidual, CostFunctor>, "IResidual");
  static_assert(std::is_base_of_v<LossFunction, LossPolicy> || std::is_void_v<LossPolicy>, "LossFunction");
  assert(images && sensorModels && 2 <= num);
  using NumericDiffCostFunc = NumericDiffCostFunction<CostFunctor, ceres::CENTRAL, 2, 3>;

  Problem::Options popt;
  popt.cost_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  popt.loss_function_ownership = DO_NOT_TAKE_OWNERSHIP;
  Problem problem(popt);

  LossFunction* lossF = (lossFunc) ? static_cast<LossFunction*>(lossFunc) : nullptr;
  std::vector<CostFunctor> costFunctors(num);
  //std::vector<NumericDiffCostFunc> costFuncs;   // no default constructor!
  std::unique_ptr<uchar[]> costFuncs(new uchar[sizeof(NumericDiffCostFunc) * num]);
  auto ptrCostFuncs = std::launder(reinterpret_cast<NumericDiffCostFunc*>(costFuncs.get()));

  for (int i = 0; i < num; ++i) {
    costFunctors[i].SetArgs(images + i, sensorModels[i]);
    auto costFunc = new (ptrCostFuncs + i)
      NumericDiffCostFunc(&costFunctors[i], DO_NOT_TAKE_OWNERSHIP);

    problem.AddResidualBlock(costFunc, lossF, reinterpret_cast<double*>(&ground));
  }
  ceres::Solve(options, &problem, &summary);

  std::destroy_n(ptrCostFuncs, num);
  return summary.IsSolutionUsable();
}
}// namespace earthxpert
