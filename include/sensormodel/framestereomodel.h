/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/stereomodel.h"
#include "core/utils//typehelper.h"

namespace earthxpert {
class ISensorModel;

/**
 *  The class FrameStereoModel implements the dedicated methods having logic
 *  specific to frame sensor systems for better perforamnce. In detail,
 *  the bundle of rays from a block of sensor geometry models are used to implement
 *  forward intersection and backward projection in the network.
 */
class EXSENSORMODEL_EXPORTS FrameStereoModel final : public StereoModel{
};
}
