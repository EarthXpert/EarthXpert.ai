/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/oeprjmarshaller.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

void OePrjMarshaller::Serialize(const std::string& oeFile, 
  FrameProject const* const oePrj) {

}

std::unique_ptr<FrameProject> 
OePrjMarshaller::Deserialize(const std::string& oeFile) {
  return {};
}
