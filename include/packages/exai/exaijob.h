/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/exai/exexai.h"
#include "core/parallel/ijob.h"

#include <iostream>
#include <string>

namespace earthxpert {
/**
 *  \brief the EXAI utils and their C API offer most of EarthXpert CLIs,
 *  and is wrapped for easier parallellism.
 */
class EXEXAI_EXPORTS ExaiJob final : public IJob {
public:
  ExaiJob(std::ostream& out = std::cout);
  ExaiJob(const ExaiJob&) = delete;
  ExaiJob(ExaiJob&&) = delete;
  ~ExaiJob() = default;

  ExaiJob& operator=(const ExaiJob&) = delete;
  ExaiJob& operator=(ExaiJob&&) = delete;

  int SetCmdArgs(int argc, char** argv) {
    mArgc = argc;
    mArgv = argv;
    return 0;
  }

  int Run() override;

  int operator()(int argc, char** argv) {
    SetCmdArgs(argc, argv);
    return Run();
  }

private:
  std::ostream& mOut;
  int mArgc;
  char** mArgv;

  std::string mCommand;

  void Help();
};
} // namespace earthxpert::thirdparty
