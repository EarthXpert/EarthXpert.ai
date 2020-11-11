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
class EXGDAL_EXPORTS GdalSrsInfo final : public GdalCmd {
public:
  ~GdalSrsInfo() = default;

  void SetCmdArgs(const int argc, char** argv) {
    mArgc = argc;
    mArgv = argv;
  }

  /**
   *  Entry point of a command
   */
  int Execute() override;

private:
  int mArgc{};
  char** mArgv{};

  void Help(const char* pszErrorMsg = nullptr);
  bool FindSRS(const char* pszInput, OGRSpatialReference& oSRS);
  CPLErr GdalSrsInfo::PrintSRS(const OGRSpatialReference& oSRS,
    const char* pszOutputType, bool bPretty, bool bPrintSep);
  void PrintSRSOutputTypes(const OGRSpatialReference& oSRS,
    const char* const* papszOutputTypes);
};
}
