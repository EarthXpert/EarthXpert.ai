/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/bgoprjmarshaller.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

void BgoPrjMarshaller::Serialize(const std::string& bgoFile, 
  FrameProject const* const bgoPrj) {

}

std::unique_ptr<FrameProject> 
BgoPrjMarshaller::Deserialize(const std::string& bgoFile) {
  return {};
}
