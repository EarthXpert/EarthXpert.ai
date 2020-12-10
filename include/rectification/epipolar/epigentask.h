/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "frameworks/pipeline/task.h"

namespace earthxpert {
/**
 *  The class EpiGenTask implements the epipolar generation task.
 */
class EXEPIPOLAR_EXPORTS EpiGenTask final : public Task {
public:
  ~EpiGenTask() = default;

  void ReadTile() override;
  void ProcessTile() override;
  void WriteTile() override;

private:
  void Initialize();
};

using UPEpiGenTask = std::unique_ptr<EpiGenTask>;
using SPEpiGenTask = std::shared_ptr<EpiGenTask>;
} // namespace earthxpert
