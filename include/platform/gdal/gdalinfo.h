/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "platform/gdal/gdalcmd.h"
#include "3rdparty/gdal/exgdal.h"

namespace earthxpert {
/**
 *
 */
class EXGDAL_EXPORTS GdalInfo final : public GdalCmd {
public:
  ~GdalInfo() = default;

  void SetCmdArgs(const int argc, char** argv) {
    mArgc = argc;
    mArgv = argv;
  }

  /**
   *  Entry point of a command
   */
  int Execute();

private:
  int mArgc{};
  char** mArgv{};

  void Help(const char* pszErrorMsg = nullptr);
  GDALInfoOptionsForBinary* GDALInfoOptionsForBinaryNew();
  void GDALInfoOptionsForBinaryFree(GDALInfoOptionsForBinary* psOptionsForBinary);
};
}
