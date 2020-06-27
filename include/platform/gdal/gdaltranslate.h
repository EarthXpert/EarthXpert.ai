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
class EXGDAL_EXPORTS GdalTranslate final : public GdalCmd {
public:
  ~GdalTranslate() = default;

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

  void Help(const char* pszErrorMsg = nullptr, int bShort = TRUE);
  GDALTranslateOptionsForBinary* GDALTranslateOptionsForBinaryNew();
  void GDALTranslateOptionsForBinaryFree(GDALTranslateOptionsForBinary* psOptionsForBinary);
};
}
