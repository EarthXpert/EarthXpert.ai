/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/project/projectmarshaller.h"
#include "project/rationalprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS RatPrjMarshaller final : public ProjectMarshaller {
public:
  ~RatPrjMarshaller() = default;

  static void Serialize(const std::string& ratFile, RationalProject const* ratPrj);
  static std::unique_ptr<RationalProject> Deserialize(const std::string& ratFile);

private:
  friend class RationalProject;
};
}
