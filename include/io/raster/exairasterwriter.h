/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/raster/irasterwriter.h"
#include "io/raster/rasterreader.h"

#include <shared_mutex>

namespace earthxpert {
/**
 *  The class ExaiRasterWriter provides functios to write into an image file
 *  with EXAI private format, and serialize pixle block in an IRaster object
 *  having specific configurations, such as size, offset, bands, etc.
 *
 *  Each ExaiRasterWriter instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to write into different subsets of bands.
 *
 *  \remark will revisit for a better design.
 */
class EXIO_EXPORTS ExaiRasterWriter : public IRasterWriter, public IRasterReader {
};

using UPExaiRasterWriter = std::unique_ptr<ExaiRasterWriter>;
using SPExaiRasterWriter = std::shared_ptr<ExaiRasterWriter>;
}
