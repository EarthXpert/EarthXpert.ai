/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/p4dprjmarshaller.h"
#include "marshaller/p4d/p4dtypes.h"
#include "3rdparty/xml/exxml.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"
#include "project/frameprj.h"

using namespace earthxpert;

void P4dPrjMarshaller::Serialize(const std::string& p4dFile, 
  FrameProject const* const p4dPrj) {

}

std::unique_ptr<FrameProject> 
P4dPrjMarshaller::Deserialize(const std::string& p4dFile) {
  UPxmlTextReader reader(
    xmlReaderForFile(p4dFile.c_str(), nullptr, 0), &xmlFreeTextReader);
  if (!reader) {
    throw std::invalid_argument("P4dPrjMarshaller: not a valid .xml");
  }

  return {};
}
