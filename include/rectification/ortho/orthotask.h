/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/ortho/exortho.h"
#include "frameworks/pipeline/task.h"

namespace earthxpert {
/**
 *  The class OrthoTask implements the ortho generation task.
 */
class EXORTHO_EXPORTS OrthoTask final : public Task {
public:
  ~OrthoTask() = default;

  void ReadTile() override;
  void ProcessTile() override;
  void WriteTile() override;

private:
  void Initialize();
};

using UPOrthoTask = std::unique_ptr<OrthoTask>;
using SPOrthoTask = std::shared_ptr<OrthoTask>;
} // namespace earthxpert
