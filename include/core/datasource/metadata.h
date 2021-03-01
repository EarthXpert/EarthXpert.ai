/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/excore.h"
#include <memory>

namespace earthxpert {
/**
 *  The class RasterMetadata provides data of the tags describing the file- and
 *  band-level information.
 */
class EXCORE_EXPORTS RasterMetadata {
};

using UPRasterMetadata = std::unique_ptr<RasterMetadata>;
using SPRasterMetadata = std::shared_ptr<RasterMetadata>;

/**
 *  The class VectorMetadata provides data of the tags describing the file- and
 *  layer-level information.
 */
class EXCORE_EXPORTS VectorMetadata {
};

using UPVectorMetadata = std::unique_ptr<VectorMetadata>;
using SPVectorMetadata = std::shared_ptr<VectorMetadata>;
} // namespace earthxpert
