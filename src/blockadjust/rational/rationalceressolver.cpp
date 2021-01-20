/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "blockadjust/rationalceressolver.h"
#include "blockadjust/rationalbbaimage.h"

#include <cassert>
#include <cmath>

using namespace earthxpert;

namespace earthxpert {
using thirdparty::IResidual;

namespace detail {
  /**
   *  \brief AnalyticDiff cost functors implements CostFunction
   */
  class RationalAnalyticG2ICostFunc final : ceres::DynamicCostFunction {

  };

  /**
   *  \brief AutoDiff cost functors implements IResidual
   *
   *  \remark requires significant changes to SensorModel
   */
  struct RationalAutoG2IResidual final : IResidual {

  };

  /**
   *  \brief NumericDiff cost functors implements IResidual
   *
   *  \question how to calculate the partial derivatives?
   *    for each parameter, pass both p & p+delta, 
   *    get two residuals r(p) & r(p+delta), then 
   *    pd = (r(p+delta) - r(p)) / delta?
   */
  struct RationalNumericG2IResidual final : IResidual {
    /**
     *  \param [in] image image observation
     *  \param [in] bbaImg pointer to image model
     */
    RationalNumericG2IResidual() {}
    RationalNumericG2IResidual(const Point2d* const image, const int64_t grdIdx,
      const RationalBbaImage* const bbaImg, const int64_t imgIdx)
      : image(image), grdIdx(grdIdx), bbaImg(bbaImg), imgIdx(imgIdx) {}
    ~RationalNumericG2IResidual() = default;

    void SetArgs(const Point2d* const image, const int64_t grdIdx,
      const RationalBbaImage* const bbaImg, const int64_t imgIdx) {
      this->image = image;
      this->grdIdx = grdIdx;
      this->bbaImg = bbaImg;
      this->imgIdx = imgIdx;
    }

    /**
     *  \brief calculate residual from the given parameters
     *
     *  \param [in] pbi block of image parameters
     *  \param [in] pbg block of ground points
     *  \param [out] residual reprojection errors of ground point at image location
     *  \return Return true if valid.
     */
    bool operator()(const double* const pbi, 
      const double* const pbg, double* residual) const {
      // update bbaImg?

      auto ground = reinterpret_cast<const Point3d* const>(pbg + grdIdx);
      Point2d ip;
      const auto ret = bbaImg->GroundToImage(*ground, ip);
      if (!ret) return false;

      residual[0] = ip.x - image->x;
      residual[1] = ip.y - image->y;
      return true;
    }

  private:
    const Point2d* image{}; // image observation
    int64_t grdIdx{};   // index of correspoding ground point scaled by 3
    const RationalBbaImage* bbaImg{};  // non-owing pointer to image model
    int64_t imgIdx{};   // index of bba image model scaled by number of model parameters
  };
}

}
