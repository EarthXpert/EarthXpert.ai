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
class EXGDAL_EXPORTS OgrInfo final : public GdalCmd{
public:
  ~OgrInfo() = default;

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

  bool bVerbose{ true };
  bool bSuperQuiet{ false };
  bool bSummaryOnly{ false };
  GIntBig nFetchFID{ OGRNullFID };
  char** papszOptions{ nullptr };

  void Help(const char* pszErrorMsg = nullptr);
  void GDALInfoPrintMetadata(GDALMajorObjectH hObject, const char* pszDomain,
     const char* pszDisplayedname, const char* pszIndent);
  void GDALInfoReportMetadata(GDALMajorObjectH hObject,
    bool bListMDD, bool bShowMetadata, char** papszExtraMDDomains);
  void ReportOnLayer(OGRLayer* poLayer, const char* pszWHERE, const char* pszGeomField,
    OGRGeometry* poSpatialFilter, bool bListMDD, bool bShowMetadata,
    char** papszExtraMDDomains, bool bFeatureCount, bool bExtent);
  void RemoveBOM(GByte* pabyData);
  void RemoveSQLComments(char*& pszSQL);
};
}
