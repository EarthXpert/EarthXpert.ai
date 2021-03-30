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
 *  The class CvRasterWriter provides functios to write into an image file
 *  with OpenCV as backend, and serialize pixle block in an IRaster object
 *  having specific configurations, such as size, offset, bands, etc.
 * 
 *  Each CvRasterWriter instance is connected and thus bound to specific bands.
 *  Therefore a new instance is needed to write into different subsets of bands.
 *
 *  \remark will revisit for a better design.
 */
class EXIO_EXPORTS CvRasterWriter : public IRasterWriter, public IRasterReader {
};

using UPCvRasterWriter = std::unique_ptr<CvRasterWriter>;
using SPCvRasterWriter = std::shared_ptr<CvRasterWriter>;
}
