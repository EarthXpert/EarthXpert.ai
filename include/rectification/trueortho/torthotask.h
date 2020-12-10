/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/trueortho/extortho.h"
#include "frameworks/pipeline/task.h"

namespace earthxpert {
/**
 *  The class OrthoTask implements the ortho generation task.
 */
class EXTORTHO_EXPORTS TorthoTask final : public Task {
public:
  ~TorthoTask() = default;

  void ReadTile() override;
  void ProcessTile() override;
  void WriteTile() override;

private:
  void Initialize();
};

using UPTorthoTask = std::unique_ptr<TorthoTask>;
using SPTorthoTask = std::shared_ptr<TorthoTask>;
} // namespace earthxpert
