/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/project/projectmarshaller.h"
#include "project/adsprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS AdsPrjMarshaller final : public ProjectMarshaller {
public:
  ~AdsPrjMarshaller() = default;

  static void Serialize(const std::string& adsFile, AdsProject const* adsPrj);
  static std::unique_ptr<AdsProject> Deserialize(const std::string& adsFile);

private:

};
}
