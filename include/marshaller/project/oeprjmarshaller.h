/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/project/projectmarshaller.h"
#include "project/frameprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS OePrjMarshaller final : public ProjectMarshaller {
public:
  ~OePrjMarshaller() = default;

  static void Serialize(const std::string& oeFile, FrameProject const* oePrj);
  static std::unique_ptr<FrameProject> Deserialize(const std::string& oeFile);

private:
  friend class FrameProject;

};
}
