/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/exai/exaicmd.h"
#include "rectification/epipolar/epipolar.h"

#include <string>
#include <vector>

namespace earthxpert {
/**
 *
 */
class EXEXAI_EXPORTS ExaiEpipolar final : public ExaiCmd {
public:
  ExaiEpipolar();
  ExaiEpipolar(const ExaiEpipolar&) = delete;
  ExaiEpipolar(ExaiEpipolar&&) = delete;
  ~ExaiEpipolar() = default;

  ExaiEpipolar& operator=(const ExaiEpipolar&) = delete;
  ExaiEpipolar& operator=(ExaiEpipolar&&) = delete;

  int SetCmdArgs(const int argc, char** argv) {
    mArgc = argc;
    mArgv = argv;
    return 0;
  }

  /**
   *  Entry point of a command
   */
  int Execute() override;

private:
  int mArgc{};
  char** mArgv{};

  void AddOptions(bpo::options_description& optDesc) override;
  void AddFrameOptions(bpo::options_description& optDesc);
  void AddRationalOptions(bpo::options_description& optDesc);
  void Help();

  int ExecuteImp() override;
};
}
