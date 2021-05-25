/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/opencv/exopencv.h"
#include "3rdparty/opencv/mat_numpy.hh"

using namespace cv;

namespace earthxpert {

namespace thirdparty {
/**
 *  \brief Apply a generic geometrical transformation to an image.
 *
 *  \param [in] mask source mask 
 */
void remap(InputArray src, InputArray mask, OutputArray dst, 
  InputArray map1, InputArray map2, const int interpolation, 
  const int borderMode, const Scalar& borderValue) {

}
}
}
