/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/projectmarshaller.h"
#include "project/frameprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS P4dPrjMarshaller final : public ProjectMarshaller {
public:
  ~P4dPrjMarshaller() = default;

  static void Serialize(const std::string& p4dFile, FrameProject const* p4dPrj);
  static void Serialize(std::ostream& outStream, FrameProject const* p4dPrj);

  static std::unique_ptr<FrameProject> Deserialize(const std::string& p4dFile);
  static std::unique_ptr<FrameProject> Deserialize(std::istream& inStream);
};
}
