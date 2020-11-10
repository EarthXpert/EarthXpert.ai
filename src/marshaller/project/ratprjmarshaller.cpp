/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/ratprjmarshaller.h"
#include "3rdparty/xml/exxml.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

void RatPrjMarshaller::Serialize(const std::string& ratFile, 
  RationalProject const* const ratPrj) {

}

std::unique_ptr<RationalProject>
RatPrjMarshaller::Deserialize(const std::string& ratFile) {
  UPxmlTextReader reader(
    xmlReaderForFile(ratFile.c_str(), nullptr, 0), &xmlFreeTextReader);
  if (!reader) 
    throw std::invalid_argument(_("RatPrjMarshaller: not a valid .xml"));

  return {};
}
