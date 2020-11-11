/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "packages/gdal/gdalcmd.h"
#include "3rdparty/gdal/exgdal.h"

namespace earthxpert {
/**
 *
 */
class EXGDAL_EXPORTS GdalInfo final : public GdalCmd {
public:
  GdalInfo();
  GdalInfo(const GdalInfo&) = delete;
  GdalInfo(GdalInfo&&) = delete;
  ~GdalInfo() = default;

  GdalInfo& operator=(const GdalInfo&) = delete;
  GdalInfo& operator=(GdalInfo&&) = delete;

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

  void Help(const char* pszErrorMsg = nullptr);
  GDALInfoOptionsForBinary* GDALInfoOptionsForBinaryNew();
  void GDALInfoOptionsForBinaryFree(GDALInfoOptionsForBinary* psOptionsForBinary);
};
}
