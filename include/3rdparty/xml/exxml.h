/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <libxml/xmlreader.h>
#include <memory>

namespace earthxpert{
using UPxmlTextReader = std::unique_ptr<xmlTextReader, decltype(&xmlFreeTextReader)>;

namespace thirdparty {
}
} // namespace earthxpert::thirdparty
