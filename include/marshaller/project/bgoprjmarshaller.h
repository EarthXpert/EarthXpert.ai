/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/project/projectmarshaller.h"
#include "project/frameprj.h"

namespace earthxpert {
class EXMARSHALLER_EXPORTS BgoPrjMarshaller final : public ProjectMarshaller {
public:
  ~BgoPrjMarshaller() = default;

  static void Serialize(const std::string& bgoFile, FrameProject const* bgoPrj);
  static std::unique_ptr<FrameProject> Deserialize(const std::string& bgoFile);

private:
  friend class FrameProject;

};
}
