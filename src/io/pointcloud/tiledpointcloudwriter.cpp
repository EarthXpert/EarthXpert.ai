/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/tiledpointcloudwriter.h"
#include "3rdparty/gettext/exgettext.h"
#include "3rdparty/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;
using pdt = earthxpert::thirdparty::PointDataTraits;
