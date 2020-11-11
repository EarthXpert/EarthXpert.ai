/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

#include <memory>

namespace earthxpert {
/**
 *  The class FeatureMarshaller serialize and deserialize point features.
 */
class EXMARSHALLER_EXPORTS FeatureMarshaller {
public:
  ~FeatureMarshaller() = default;
};

using UPFeatureMarshaller = std::unique_ptr<FeatureMarshaller>;
using SPFeatureMarshaller = std::shared_ptr<FeatureMarshaller>;
}
