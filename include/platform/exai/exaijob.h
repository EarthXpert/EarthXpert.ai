/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "platform/exai/exexai.h"
#include "core/parallel/ijob.h"

#include <string>

namespace earthxpert {
/**
 *  \brief the EXAI utils and their C API offer most of EarthXpert CLIs,
 *  and is wrapped for easier parallellism.
 */
class EXEXAI_EXPORTS ExaiJob final : public IJob {
public:
  ExaiJob();
  ExaiJob(const ExaiJob&) = delete;
  ExaiJob(ExaiJob&&) = delete;
  ~ExaiJob() = default;

  ExaiJob& operator=(const ExaiJob&) = delete;
  ExaiJob& operator=(ExaiJob&&) = delete;

  int SetArgs();

  int Run() override;

private:
  std::string mCommand;
};
} // namespace earthxpert::thirdparty
