/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include "core/datasource/pointcloud.h"

namespace earthxpert {
FieldGroup EXCORE_EXPORTS FormatToFieldGroup(int fmt) noexcept;
int EXCORE_EXPORTS FieldGroupToFormat(FieldGroup fieldGroup) noexcept;
}
