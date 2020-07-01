/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once 

#include "platform/gdal/exgdal.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/parallel/ijob.h"

#include <iostream>

namespace earthxpert {
/**
 *  \brief the GDAL utils and their C API offer a rich set of raster and vector CLIs, 
 *  and is wrapped for easier customization.
 */
class EXGDAL_EXPORTS GdalJob final : public IJob {
public:
  GdalJob(std::ostream& out = std::cout);
  GdalJob(const GdalJob&) = delete;
  GdalJob(GdalJob&&) = delete;
  ~GdalJob() = default;

  GdalJob& operator=(const GdalJob&) = delete;
  GdalJob& operator=(GdalJob&&) = delete;

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
