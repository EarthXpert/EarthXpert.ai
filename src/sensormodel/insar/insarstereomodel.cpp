/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/insarstereomodel.h"

using namespace earthxpert;

SarStereoModel::SarStereoModel(
  SarModel const* const refModel, SarModel const* const depModel) 
  : StereoModel(static_cast<const ISensorModel*>(refModel),
  static_cast<const ISensorModel*>(depModel)) {
  mRefModel = refModel;
  mDepModel = depModel;
}
