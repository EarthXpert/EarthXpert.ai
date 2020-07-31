/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/p4dprjmarshaller.h"
#include "project/frameprj.h"

#include <libxml/xmlreader.h>

using namespace earthxpert;

void P4dPrjMarshaller::Serialize(const std::string& p4dFile, 
  FrameProject const* const p4dPrj) {

}

void P4dPrjMarshaller::Serialize(std::ostream& outStream,
  FrameProject const* const p4dPrj) {

}

std::unique_ptr<FrameProject> P4dPrjMarshaller::Deserialize(const std::string& p4dFile) {
  return {};
}

std::unique_ptr<FrameProject> P4dPrjMarshaller::Deserialize(std::istream& inStream) {
  return {};
}
