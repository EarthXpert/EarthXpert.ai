/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "imageproc/algorithm/imgenhance.h"
#include "core/datasource/cvraster.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

namespace earthxpert {
typedef void (*WallisFunc)(cv::Mat& mat, const WallisParams* wp);

template<typename Ty> void 
WallisFilter(cv::Mat& mat, const WallisParams* wp) {
  const int numBands = mat.channels();
}

/**
 *  \brief normalized local contrast enhancement.
 */
bool WallisFilterCv(CvRaster* raster, const WallisParams* wp) {
  static WallisFunc funcs[kNumDepth] = { WallisFilter<uchar>, 
    WallisFilter<char>, WallisFilter<ushort>, WallisFilter<short>,
    WallisFilter<int>, WallisFilter<float>, WallisFilter<double>, nullptr };

  const int depth = CV_MAT_DEPTH(raster->GetDataType());
  auto func = funcs[depth];

  auto& mat = raster->GetCvMat();
  func(mat, wp);

  return true;
}
}
