/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "project/rationalprjimage.h"

#include <cassert>

using namespace earthxpert;

RationalPrjImage::RationalPrjImage(std::string imgPath) 
  : mPath(std::move(imgPath)) {

}
