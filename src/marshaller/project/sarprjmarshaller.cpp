/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/sarprjmarshaller.h"
#include "3rdparty/xml/exxml.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

void SarPrjMarshaller::Serialize(const std::string& sarFile, 
  SarProject const* const sarPrj) {

}

std::unique_ptr<SarProject>
SarPrjMarshaller::Deserialize(const std::string& sarFile) {
  UPxmlTextReader reader(
    xmlReaderForFile(sarFile.c_str(), nullptr, 0), &xmlFreeTextReader);
  if (!reader) 
    throw std::invalid_argument(_("SarPrjMarshaller: not a valid .xml"));

  return {};
}
