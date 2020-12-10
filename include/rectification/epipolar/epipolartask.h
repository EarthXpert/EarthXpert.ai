/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "rectification/epipolar/exepipolar.h"
#include "frameworks/pipeline/task.h"

namespace earthxpert {
/**
 *  The class EpipolarTask implements the epipolar generation task.
 */
class EXEPIPOLAR_EXPORTS EpipolarTask final : public Task {
public:
  ~EpipolarTask() = default;

  void ReadTile() override;
  void ProcessTile() override;
  void WriteTile() override;

private:
  void Initialize();
};

using UPEpipolarTask = std::unique_ptr<EpipolarTask>;
using SPEpipolarTask = std::shared_ptr<EpipolarTask>;
} // namespace earthxpert
