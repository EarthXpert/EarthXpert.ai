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
   */
  struct RationalNumericG2IResidual final : IResidual {
    /**
     *  \param [in] image image observation
     *  \param [in] bbaImg pointer to image model
     */
    RationalNumericG2IResidual() {}
    RationalNumericG2IResidual(const Point2d* const image, const int64_t grdIdx,
      const RationalBbaImage* const bbaImg, const int64_t bbaIdx)
      : image(image), grdIdx(grdIdx), bbaImg(bbaImg), bbaIdx(bbaIdx) {}
    ~RationalNumericG2IResidual() = default;

    void SetArgs(const Point2d* const image, const int64_t grdIdx,
      const RationalBbaImage* const bbaImg, const int64_t bbaIdx) {
      this->image = image;
      this->grdIdx = grdIdx;
      this->bbaImg = bbaImg;
      this->bbaIdx = bbaIdx;
    }

    /**
     *  \brief calculate residual from the given parameters
     *
     *  \param [in] pbi block of image parameters
     *  \param [in] pbp block of ground points
     *  \param [out] residual reprojection errors of ground point at image location
     *  \return Return true if valid.
     */
    bool operator()(const double* const pbi, 
      const double* const pbp, double* residual) const {
      // update bbaImg?

      auto ground = reinterpret_cast<const Point3d* const>(pbp + grdIdx);
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
    int64_t bbaIdx{};   // index of bba image model scaled by number of model parameters
  };
}
}
