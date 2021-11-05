/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/project/adsprjmarshaller.h"
#include "3rdparty/xml/exxml.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stlutils.h"

using namespace earthxpert;

void AdsPrjMarshaller::Serialize(const std::string& adsFile,
  AdsProject const* const adsPrj) {

}

std::unique_ptr<AdsProject>
AdsPrjMarshaller::Deserialize(const std::string& adsFile) {
  return {};
}
