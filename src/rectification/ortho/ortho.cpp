/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/ortho/ortho.h"
#include "core/utils/fsutils.h"
#include "core/utils/rasterresample.h"
#include "io/raster/rasterreader.h"
#include "io/raster/rasterwriter.h"
#include "marshaller/sensormodel/rationalmodelmarshaller.h"
//#include "rectification/ortho/orthogenerator.h"
#include "sensormodel/rationalmodel.h"

#include <boost/format.hpp>
#include <iostream>
#include <sstream>

using namespace earthxpert;
using namespace earthxpert::utils;

namespace earthxpert {
namespace detail {
/**
 *  \return Return file path of the generated epipolar image
 */
std::string GenerateOrtho() {
  return {};
}
}

std::string ToString(const OrthoOptions* oOpt) {
  std::stringstream ss;

  return ss.str();
}

/**
 *  \brief Generate ortho images for an image having frame models.
 */
void GenerateFrameOrtho() {
}

/**
 *  \brief Generate ortho images for an image having rational models.
 */
void GenerateRationalOrtho(const OrthoOptions& oOpt) {
}
}
