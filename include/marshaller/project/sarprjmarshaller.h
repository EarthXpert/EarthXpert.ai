/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/project/projectmarshaller.h"
#include "project/sarprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS SarPrjMarshaller final : public ProjectMarshaller {
public:
  ~SarPrjMarshaller() = default;

  static void Serialize(const std::string& sarFile, SarProject const* sarPrj);
  static std::unique_ptr<SarProject> Deserialize(const std::string& sarFile);

private:
  friend class SarProject;

};
}
