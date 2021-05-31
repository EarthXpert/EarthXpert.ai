/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/formats/support/supportdata.h"

namespace earthxpert {
/**
 *  The class
 */
class EXCORE_EXPORTS RfmSupportData final : public SupportData {
public:
  ~RfmSupportData() = default;

  std::string ResolveModelFile(const std::string& imgFile) const;
};
}
