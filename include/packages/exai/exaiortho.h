/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/exai/exaicmd.h"
//#include "rectification/ortho/ortho.h"

#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace bpo = boost::program_options;

namespace earthxpert {
/**
 *
 */
class EXEXAI_EXPORTS ExaiOrtho final : public ExaiCmd {
public:
  ExaiOrtho();
  ExaiOrtho(const ExaiOrtho&) = delete;
  ExaiOrtho(ExaiOrtho&&) = delete;
  ~ExaiOrtho() = default;

  ExaiOrtho& operator=(const ExaiOrtho&) = delete;
  ExaiOrtho& operator=(ExaiOrtho&&) = delete;

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

  void AddFrameOptions(bpo::options_description& optDesc);
  void AddRationalOptions(bpo::options_description& optDesc);
  void Help();
};
}
