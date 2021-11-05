/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/stereomodel.h"
#include "core/utils//typehelper.h"
#include "sensormodel/sarmodel.h"

namespace earthxpert {
/**
 *
 */
class EXSENSORMODEL_EXPORTS SarStereoModel /*final*/ : public StereoModel {
public:
  SarStereoModel(SarModel const* refModel, SarModel const* depModel);

private:
  const SarModel* mRefModel{};  // reference/master
  const SarModel* mDepModel{};  // dependent/slave
};
}
