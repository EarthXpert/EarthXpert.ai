/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/exai/exaicmd.h"

#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace bpo = boost::program_options;

namespace earthxpert {
/**
 *
 */
class EXEXAI_EXPORTS ExaiBlockAdjust final : public ExaiCmd {
public:
  ExaiBlockAdjust();
  ExaiBlockAdjust(const ExaiBlockAdjust&) = delete;
  ExaiBlockAdjust(ExaiBlockAdjust&&) = delete;
  ~ExaiBlockAdjust();

  ExaiBlockAdjust& operator=(const ExaiBlockAdjust&) = delete;
  ExaiBlockAdjust& operator=(ExaiBlockAdjust&&) = delete;

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

  void Help();
};
}
