/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
/******************************************************************************
 *
 * Project:  OpenGIS Simple Features Reference Implementation
 * Purpose:  Simple client for viewing OGR driver data.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 * Copyright (c) 2008-2013, Even Rouault <even dot rouault at mines-paris dot org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/
#include "platform/gdal/gdalcmds.h"

#include "commonutils.h"
#include "cpl_conv.h"
#include "cpl_error.h"
#include "cpl_multiproc.h"
#include "cpl_port.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal_version.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "gdal_version.h"
#include "ogr_api.h"
#include "ogr_core.h"
#include "ogr_feature.h"
#include "ogr_geometry.h"
#include "ogr_p.h"
#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>

CPL_CVSID("$Id: ogrinfo.cpp a6d7833fdb738661c118e43c577d959652e4b168 2018-04-20 19:24:15 +0200 Even Rouault $")

namespace earthxpert {
namespace detail {
bool bVerbose = true;
bool bSuperQuiet = false;
bool bSummaryOnly = false;
GIntBig nFetchFID = OGRNullFID;
char** papszOptions = nullptr;

void HelpOgrInfo(const char* pszErrorMsg = nullptr) {
  printf("\nUsage: exgdal ogrinfo [--help-general] [-ro] [-q] [-where restricted_where|@filename]\n"
    "               [-spat xmin ymin xmax ymax] [-geomfield field] [-fid fid]\n"
    "               [-sql statement|@filename] [-dialect sql_dialect] [-al] [-rl] [-so] [-fields={YES/NO}]\n"
    "               [-geom={YES/NO/SUMMARY}] [[-oo NAME=VALUE] ...]\n"
    "               [-nomd] [-listmdd] [-mdd domain|`all`]*\n"
    "               [-nocount] [-noextent]\n"
    "               datasource_name [layer [layer ...]]\n");

  if (pszErrorMsg)
    fprintf(stderr, "\nFAILURE: %s\n", pszErrorMsg);

  exit(1);
}

void GDALInfoPrintMetadata(GDALMajorObjectH hObject,
  const char* pszDomain, const char* pszDisplayedname, const char* pszIndent) {
  bool bIsxml = false;

  if (pszDomain != nullptr && STARTS_WITH_CI(pszDomain, "xml:"))
    bIsxml = true;

  char** papszMetadata = GDALGetMetadata(hObject, pszDomain);
  if (CSLCount(papszMetadata) > 0) {
    printf("%s%s:\n", pszIndent, pszDisplayedname);
    for (int i = 0; papszMetadata[i] != nullptr; i++)
    {
      if (bIsxml)
        printf("%s%s\n", pszIndent, papszMetadata[i]);
      else
        printf("%s  %s\n", pszIndent, papszMetadata[i]);
    }
  }
}

void GDALInfoReportMetadata(GDALMajorObjectH hObject,
  bool bListMDD, bool bShowMetadata, char** papszExtraMDDomains) {
  const char* pszIndent = "";

  /* -------------------------------------------------------------------- */
  /*      Report list of Metadata domains                                 */
  /* -------------------------------------------------------------------- */
  if (bListMDD) {
    char** papszMDDList = GDALGetMetadataDomainList(hObject);
    char** papszIter = papszMDDList;

    if (papszMDDList != nullptr)
      printf("%sMetadata domains:\n", pszIndent);
    while (papszIter && *papszIter) {
      if (EQUAL(*papszIter, ""))
        printf("%s  (default)\n", pszIndent);
      else
        printf("%s  %s\n", pszIndent, *papszIter);
      papszIter++;
    }
    CSLDestroy(papszMDDList);
  }

  if (!bShowMetadata) return;

  /* -------------------------------------------------------------------- */
  /*      Report default Metadata domain.                                 */
  /* -------------------------------------------------------------------- */
  GDALInfoPrintMetadata(hObject, nullptr, "Metadata", pszIndent);

  /* -------------------------------------------------------------------- */
  /*      Report extra Metadata domains                                   */
  /* -------------------------------------------------------------------- */
  if (papszExtraMDDomains != nullptr) {
    char** papszExtraMDDomainsExpanded = nullptr;

    if (EQUAL(papszExtraMDDomains[0], "all") && !papszExtraMDDomains[1]) {
      char** papszMDDList = GDALGetMetadataDomainList(hObject);
      char** papszIter = papszMDDList;

      while (papszIter != nullptr && *papszIter != nullptr) {
        if (!EQUAL(*papszIter, "")) {
          papszExtraMDDomainsExpanded = CSLAddString(papszExtraMDDomainsExpanded, *papszIter);
        }
        papszIter++;
      }
      CSLDestroy(papszMDDList);
    }
    else {
      papszExtraMDDomainsExpanded = CSLDuplicate(papszExtraMDDomains);
    }

    for (int iMDD = 0;
      papszExtraMDDomainsExpanded != nullptr &&
      papszExtraMDDomainsExpanded[iMDD] != nullptr; iMDD++) {
      char pszDisplayedname[256];
      snprintf(pszDisplayedname, 256, "Metadata (%s)",
        papszExtraMDDomainsExpanded[iMDD]);
      GDALInfoPrintMetadata(hObject, papszExtraMDDomainsExpanded[iMDD],
        pszDisplayedname, pszIndent);
    }

    CSLDestroy(papszExtraMDDomainsExpanded);
  }
}

void ReportOnLayer(OGRLayer* poLayer, const char* pszWHERE,
  const char* pszGeomField, OGRGeometry* poSpatialFilter,
  bool bListMDD, bool bShowMetadata, char** papszExtraMDDomains,
  bool bFeatureCount, bool bExtent) {
  OGRFeatureDefn* poDefn = poLayer->GetLayerDefn();

  /* -------------------------------------------------------------------- */
  /*      Set filters if provided.                                        */
  /* -------------------------------------------------------------------- */
  if (pszWHERE) {
    if (poLayer->SetAttributeFilter(pszWHERE) != OGRERR_NONE) {
      printf("FAILURE: SetAttributeFilter(%s) failed.\n", pszWHERE);
      exit(1);
    }
  }

  if (poSpatialFilter) {
    if (pszGeomField) {
      const int iGeomField = poDefn->GetGeomFieldIndex(pszGeomField);
      if (iGeomField >= 0)
        poLayer->SetSpatialFilter(iGeomField, poSpatialFilter);
      else
        printf("WARNING: Cannot find geometry field %s.\n",
          pszGeomField);
    }
    else {
      poLayer->SetSpatialFilter(poSpatialFilter);
    }
  }

  /* -------------------------------------------------------------------- */
  /*      Report various overall information.                             */
  /* -------------------------------------------------------------------- */
  if (!bSuperQuiet) {
    printf("\n");
    printf("Layer name: %s\n", poLayer->GetName());
  }

  GDALInfoReportMetadata(static_cast<GDALMajorObjectH>(poLayer),
    bListMDD, bShowMetadata, papszExtraMDDomains);

  if (bVerbose) {
    const int nGeomFieldCount = poLayer->GetLayerDefn()->GetGeomFieldCount();
    if (nGeomFieldCount > 1) {
      for (int iGeom = 0; iGeom < nGeomFieldCount; iGeom++) {
        OGRGeomFieldDefn* poGFldDefn =
          poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
        printf("Geometry (%s): %s\n", poGFldDefn->GetNameRef(),
          OGRGeometryTypeToName(poGFldDefn->GetType()));
      }
    }
    else {
      printf("Geometry: %s\n", OGRGeometryTypeToName(poLayer->GetGeomType()));
    }

    if (bFeatureCount)
      printf("Feature Count: " CPL_FRMT_GIB "\n", poLayer->GetFeatureCount());

    OGREnvelope oExt;
    if (bExtent && nGeomFieldCount > 1) {
      for (int iGeom = 0; iGeom < nGeomFieldCount; iGeom++) {
        if (poLayer->GetExtent(iGeom, &oExt, TRUE) == OGRERR_NONE) {
          OGRGeomFieldDefn* poGFldDefn =
            poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
          CPLprintf("Extent (%s): (%f, %f) - (%f, %f)\n",
            poGFldDefn->GetNameRef(), oExt.MinX, oExt.MinY, oExt.MaxX, oExt.MaxY);
        }
      }
    }
    else if (bExtent && poLayer->GetExtent(&oExt, TRUE) == OGRERR_NONE) {
      CPLprintf("Extent: (%f, %f) - (%f, %f)\n",
        oExt.MinX, oExt.MinY, oExt.MaxX, oExt.MaxY);
    }

    if (nGeomFieldCount > 1) {
      for (int iGeom = 0; iGeom < nGeomFieldCount; iGeom++) {
        OGRGeomFieldDefn* poGFldDefn =
          poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
        OGRSpatialReference* poSRS = poGFldDefn->GetSpatialRef();
        char* pszWKT = nullptr;
        if (!poSRS) {
          pszWKT = CPLStrdup("(unknown)");
        }
        else {
          poSRS->exportToPrettyWkt(&pszWKT);
        }

        printf("SRS WKT (%s):\n%s\n", poGFldDefn->GetNameRef(), pszWKT);
        CPLFree(pszWKT);
      }
    }
    else {
      char* pszWKT = nullptr;
      if (poLayer->GetSpatialRef() == nullptr) {
        pszWKT = CPLStrdup("(unknown)");
      }
      else {
        poLayer->GetSpatialRef()->exportToPrettyWkt(&pszWKT);
      }

      printf("Layer SRS WKT:\n%s\n", pszWKT);
      CPLFree(pszWKT);
    }

    if (strlen(poLayer->GetFIDColumn()) > 0)
      printf("FID Column = %s\n", poLayer->GetFIDColumn());

    for (int iGeom = 0; iGeom < nGeomFieldCount; iGeom++) {
      OGRGeomFieldDefn* poGFldDefn =
        poLayer->GetLayerDefn()->GetGeomFieldDefn(iGeom);
      if (nGeomFieldCount == 1 && EQUAL(poGFldDefn->GetNameRef(), "") && poGFldDefn->IsNullable()) {
        break;
      }
      printf("Geometry Column ");
      if (nGeomFieldCount > 1)
        printf("%d ", iGeom + 1);
      if (!poGFldDefn->IsNullable())
        printf("NOT NULL ");
      printf("= %s\n", poGFldDefn->GetNameRef());
    }

    for (int iAttr = 0; iAttr < poDefn->GetFieldCount(); iAttr++) {
      OGRFieldDefn* poField = poDefn->GetFieldDefn(iAttr);
      const char* pszType = (poField->GetSubType() != OFSTNone)
        ? CPLSPrintf("%s(%s)",
          poField->GetFieldTypeName(poField->GetType()),
          poField->GetFieldSubTypeName(poField->GetSubType()))
        : poField->GetFieldTypeName(poField->GetType());
      printf("%s: %s (%d.%d)", poField->GetNameRef(),
        pszType, poField->GetWidth(), poField->GetPrecision());
      if (!poField->IsNullable())
        printf(" NOT NULL");
      if (poField->GetDefault() != nullptr)
        printf(" DEFAULT %s", poField->GetDefault());
      printf("\n");
    }
  }

  /* -------------------------------------------------------------------- */
  /*      Read, and dump features.                                        */
  /* -------------------------------------------------------------------- */
  if (nFetchFID == OGRNullFID && !bSummaryOnly) {
    OGRFeature* poFeature = nullptr;
    while (poFeature = poLayer->GetNextFeature()) {
      if (!bSuperQuiet)
        poFeature->DumpReadable(nullptr, papszOptions);
      OGRFeature::DestroyFeature(poFeature);
    }
  }
  else if (nFetchFID != OGRNullFID) {
    OGRFeature* poFeature = poLayer->GetFeature(nFetchFID);
    if (!poFeature) {
      printf("Unable to locate feature id " CPL_FRMT_GIB
        " on this layer.\n",
        nFetchFID);
    }
    else {
      poFeature->DumpReadable(nullptr, papszOptions);
      OGRFeature::DestroyFeature(poFeature);
    }
  }
}

/* Remove potential UTF-8 BOM from data (must be NUL terminated) */
void RemoveBOM(GByte* pabyData) {
  if (pabyData[0] == 0xEF && pabyData[1] == 0xBB && pabyData[2] == 0xBF)
  {
    memmove(pabyData, pabyData + 3,
      strlen(reinterpret_cast<char*>(pabyData) + 3) + 1);
  }
}

void RemoveSQLComments(char*& pszSQL) {
  char** papszLines = CSLTokenizeStringComplex(pszSQL, "\r\n", FALSE, FALSE);
  CPLString osSQL;
  for (char** papszIter = papszLines; papszIter && *papszIter; ++papszIter) {
    if (!STARTS_WITH(*papszIter, "--")) {
      osSQL += *papszIter;
      osSQL += " ";
    }
  }
  CSLDestroy(papszLines);
  CPLFree(pszSQL);
  pszSQL = CPLStrdup(osSQL);
}
}

/**
 *
 */
#define CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(nExtraArg) \
    do { if (iArg + nExtraArg >= argc) \
        detail::HelpOgrInfo(CPLSPrintf("%s option requires %d argument(s)", \
                         argv[iArg], nExtraArg)); } while( false )

int OgrInfo(int argc, char** argv)
{
  // Check strict compilation and runtime library version as we use C++ API.
  if (!GDAL_CHECK_VERSION(argv[0]))
    exit(1);

  EarlySetConfigOptions(argc, argv);

  OGRRegisterAll();

  /* -------------------------------------------------------------------- */
  /*      Processing command line arguments.                              */
  /* -------------------------------------------------------------------- */
  argc = OGRGeneralCmdLineProcessor(argc, &argv, 0);

  if (argc < 1)
    exit(-argc);

  char* pszWHERE = nullptr;
  const char* pszDataSource = nullptr;
  char** papszLayers = nullptr;
  OGRGeometry* poSpatialFilter = nullptr;
  int nRepeatCount = 1;
  bool bAllLayers = false;
  char* pszSQLStatement = nullptr;
  const char* pszDialect = nullptr;
  int nRet = 0;
  const char* pszGeomField = nullptr;
  char** papszOpenOptions = nullptr;
  char** papszExtraMDDomains = nullptr;
  bool bListMDD = false;
  bool bShowMetadata = true;
  bool bFeatureCount = true;
  bool bExtent = true;
  bool bDatasetGetNextFeature = false;
  bool bReadOnly = false;
  bool bUpdate = false;

  for (int iArg = 1; iArg < argc; iArg++) {
    if (EQUAL(argv[iArg], "--utility_version")) {
      printf("%s was compiled against GDAL %s and "
        "is running against GDAL %s\n",
        argv[0], GDAL_RELEASE_NAME,
        GDALVersionInfo("RELEASE_NAME"));
      CSLDestroy(argv);
      return 0;
    }
    else if (EQUAL(argv[iArg], "--help")) {
      detail::HelpOgrInfo();
    }
    else if (EQUAL(argv[iArg], "-ro")) {
      bReadOnly = true;
    }
    else if (EQUAL(argv[iArg], "-update")) {
      bUpdate = true;
    }
    else if (EQUAL(argv[iArg], "-q") || EQUAL(argv[iArg], "-quiet")) {
      detail::bVerbose = false;
    }
    else if (EQUAL(argv[iArg], "-qq")) {
      /* Undocumented: mainly only useful for AFL testing */
      detail::bVerbose = false;
      detail::bSuperQuiet = true;
    }
    else if (EQUAL(argv[iArg], "-fid")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      detail::nFetchFID = CPLAtoGIntBig(argv[++iArg]);
    }
    else if (EQUAL(argv[iArg], "-spat")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(4);

      OGRLinearRing oRing;
      oRing.addPoint(CPLAtof(argv[iArg + 1]),
        CPLAtof(argv[iArg + 2]));
      oRing.addPoint(CPLAtof(argv[iArg + 1]),
        CPLAtof(argv[iArg + 4]));
      oRing.addPoint(CPLAtof(argv[iArg + 3]),
        CPLAtof(argv[iArg + 4]));
      oRing.addPoint(CPLAtof(argv[iArg + 3]),
        CPLAtof(argv[iArg + 2]));
      oRing.addPoint(CPLAtof(argv[iArg + 1]),
        CPLAtof(argv[iArg + 2]));

      poSpatialFilter = new OGRPolygon();
      static_cast<OGRPolygon*>(poSpatialFilter)->addRing(&oRing);
      iArg += 4;
    }
    else if (EQUAL(argv[iArg], "-geomfield")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      pszGeomField = argv[++iArg];
    }
    else if (EQUAL(argv[iArg], "-where")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      iArg++;
      CPLFree(pszWHERE);
      GByte* pabyRet = nullptr;
      if (argv[iArg][0] == '@' &&
        VSIIngestFile(nullptr, argv[iArg] + 1, &pabyRet, nullptr, 1024 * 1024)) {
        detail::RemoveBOM(pabyRet);
        pszWHERE = reinterpret_cast<char*>(pabyRet);
      }
      else {
        pszWHERE = CPLStrdup(argv[iArg]);
      }
    }
    else if (EQUAL(argv[iArg], "-sql")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      iArg++;
      CPLFree(pszSQLStatement);
      GByte* pabyRet = nullptr;
      if (argv[iArg][0] == '@' &&
        VSIIngestFile(nullptr, argv[iArg] + 1, &pabyRet, nullptr, 1024 * 1024)) {
        detail::RemoveBOM(pabyRet);
        pszSQLStatement = reinterpret_cast<char*>(pabyRet);
        detail::RemoveSQLComments(pszSQLStatement);
      }
      else {
        pszSQLStatement = CPLStrdup(argv[iArg]);
      }
    }
    else if (EQUAL(argv[iArg], "-dialect")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      pszDialect = argv[++iArg];
    }
    else if (EQUAL(argv[iArg], "-rc")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      nRepeatCount = atoi(argv[++iArg]);
    }
    else if (EQUAL(argv[iArg], "-al")) {
      bAllLayers = true;
    }
    else if (EQUAL(argv[iArg], "-so") || EQUAL(argv[iArg], "-summary")) {
      detail::bSummaryOnly = true;
    }
    else if (STARTS_WITH_CI(argv[iArg], "-fields=")) {
      char* pszTemp =
        static_cast<char*>(CPLMalloc(32 + strlen(argv[iArg])));
      snprintf(pszTemp,
        32 + strlen(argv[iArg]),
        "DISPLAY_FIELDS=%s", argv[iArg] + strlen("-fields="));
      detail::papszOptions = CSLAddString(detail::papszOptions, pszTemp);
      CPLFree(pszTemp);
    }
    else if (STARTS_WITH_CI(argv[iArg], "-geom=")) {
      char* pszTemp =
        static_cast<char*>(CPLMalloc(32 + strlen(argv[iArg])));
      snprintf(pszTemp,
        32 + strlen(argv[iArg]),
        "DISPLAY_GEOMETRY=%s", argv[iArg] + strlen("-geom="));
      detail::papszOptions = CSLAddString(detail::papszOptions, pszTemp);
      CPLFree(pszTemp);
    }
    else if (EQUAL(argv[iArg], "-oo")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      papszOpenOptions = CSLAddString(papszOpenOptions,
        argv[++iArg]);
    }
    else if (EQUAL(argv[iArg], "-nomd")) {
      bShowMetadata = false;
    }
    else if (EQUAL(argv[iArg], "-listmdd")) {
      bListMDD = true;
    }
    else if (EQUAL(argv[iArg], "-mdd")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      papszExtraMDDomains = CSLAddString(papszExtraMDDomains,
        argv[++iArg]);
    }
    else if (EQUAL(argv[iArg], "-nocount")) {
      bFeatureCount = false;
    }
    else if (EQUAL(argv[iArg], "-noextent")) {
      bExtent = false;
    }
    else if (EQUAL(argv[iArg], "-rl")) {
      bDatasetGetNextFeature = true;
    }
    else if (argv[iArg][0] == '-') {
    detail::HelpOgrInfo(CPLSPrintf("Unknown option name '%s'", argv[iArg]));
    }
    else if (pszDataSource == nullptr) {
      pszDataSource = argv[iArg];
    }
    else {
      papszLayers = CSLAddString(papszLayers, argv[iArg]);
      bAllLayers = false;
    }
  }

  if (pszDataSource == nullptr)
    detail::HelpOgrInfo("No datasource specified.");

  if (pszDialect != nullptr && pszWHERE != nullptr &&
    pszSQLStatement == nullptr)
    printf("Warning: -dialect is ignored with -where. Use -sql instead");

  if (bDatasetGetNextFeature && pszSQLStatement) {
    detail::HelpOgrInfo("-rl is incompatible with -sql");
  }

#ifdef __AFL_HAVE_MANUAL_CONTROL
  while (__AFL_LOOP(1000)) {
#endif
    /* -------------------------------------------------------------------- */
    /*      Open data source.                                               */
    /* -------------------------------------------------------------------- */
    GDALDataset* poDS = static_cast<GDALDataset*>(GDALOpenEx(
      pszDataSource, ((bReadOnly || pszSQLStatement == nullptr) &&
        !bUpdate ? GDAL_OF_READONLY : GDAL_OF_UPDATE) | GDAL_OF_VECTOR,
      nullptr, papszOpenOptions, nullptr));

    if (poDS == nullptr && !bReadOnly && !bUpdate && !pszSQLStatement) {
      // In some cases (empty geopackage for example), opening in read-only
      // mode fails, so retry in update mode
      if (GDALIdentifyDriverEx(pszDataSource, GDAL_OF_VECTOR, nullptr, nullptr)) {
        poDS = static_cast<GDALDataset*>(GDALOpenEx(
          pszDataSource,
          GDAL_OF_UPDATE | GDAL_OF_VECTOR, nullptr,
          papszOpenOptions, nullptr));
      }
    }
    if (poDS == nullptr && !bReadOnly && !bUpdate && pszSQLStatement != nullptr) {
      poDS = static_cast<GDALDataset*>(GDALOpenEx(
        pszDataSource, GDAL_OF_READONLY | GDAL_OF_VECTOR, nullptr,
        papszOpenOptions, nullptr));
      if (poDS != nullptr && detail::bVerbose) {
        printf("Had to open data source read-only.\n");
#ifdef __AFL_HAVE_MANUAL_CONTROL
        bReadOnly = true;
#endif
      }
    }

    GDALDriver* poDriver = nullptr;
    if (poDS != nullptr)
      poDriver = poDS->GetDriver();

    /* -------------------------------------------------------------------- */
    /*      Report failure                                                  */
    /* -------------------------------------------------------------------- */
    if (!poDS) {
      printf("FAILURE:\n"
        "Unable to open datasource `%s' with the following drivers.\n",
        pszDataSource);
#ifdef __AFL_HAVE_MANUAL_CONTROL
      continue;
#else
      OGRSFDriverRegistrar* poR = OGRSFDriverRegistrar::GetRegistrar();
      for (int iDriver = 0; iDriver < poR->GetDriverCount(); iDriver++) {
        printf("  -> %s\n", poR->GetDriver(iDriver)->GetDescription());
      }

      nRet = 1;
      goto end;
#endif
    }

    CPLAssert(poDriver != nullptr);

    /* -------------------------------------------------------------------- */
    /*      Some information messages.                                      */
    /* -------------------------------------------------------------------- */
    if (detail::bVerbose)
      printf("INFO: Open of `%s'\n"
        "      using driver `%s' successful.\n",
        pszDataSource, poDriver->GetDescription());

    if (detail::bVerbose && !EQUAL(pszDataSource, poDS->GetDescription())) {
      printf("INFO: Internal data source name `%s'\n"
        "      different from user name `%s'.\n",
        poDS->GetDescription(), pszDataSource);
    }

    detail::GDALInfoReportMetadata(static_cast<GDALMajorObjectH>(poDS),
      bListMDD, bShowMetadata, papszExtraMDDomains);

    if (bDatasetGetNextFeature) {
      nRepeatCount = 0;  // skip layer reporting.

/* -------------------------------------------------------------------- */
/*      Set filters if provided.                                        */
/* -------------------------------------------------------------------- */
      if (pszWHERE != nullptr || poSpatialFilter != nullptr) {
        for (int iLayer = 0; iLayer < poDS->GetLayerCount(); iLayer++) {
          OGRLayer* poLayer = poDS->GetLayer(iLayer);

          if (poLayer == nullptr) {
            printf("FAILURE: Couldn't fetch advertised layer %d!\n",
              iLayer);
            exit(1);
          }

          if (pszWHERE != nullptr) {
            if (poLayer->SetAttributeFilter(pszWHERE) != OGRERR_NONE) {
              printf("WARNING: SetAttributeFilter(%s) "
                "failed on layer %s.\n",
                pszWHERE, poLayer->GetName());
            }
          }

          if (poSpatialFilter != nullptr) {
            if (pszGeomField != nullptr) {
              OGRFeatureDefn* poDefn = poLayer->GetLayerDefn();
              const int iGeomField =
                poDefn->GetGeomFieldIndex(pszGeomField);
              if (iGeomField >= 0)
                poLayer->SetSpatialFilter(iGeomField,
                  poSpatialFilter);
              else
                printf("WARNING: Cannot find geometry field %s.\n",
                  pszGeomField);
            }
            else {
              poLayer->SetSpatialFilter(poSpatialFilter);
            }
          }
          }
        }

      std::set<OGRLayer*> oSetLayers;
      while (true) {
        OGRLayer* poLayer = nullptr;
        OGRFeature* poFeature = poDS->GetNextFeature(&poLayer, nullptr,
          nullptr, nullptr);
        if (poFeature == nullptr)
          break;
        if (papszLayers == nullptr || poLayer == nullptr ||
          CSLFindString(papszLayers, poLayer->GetName()) >= 0) {
          if (detail::bVerbose && poLayer != nullptr &&
            oSetLayers.find(poLayer) == oSetLayers.end()) {
            oSetLayers.insert(poLayer);
            const bool bSummaryOnlyBackup = detail::bSummaryOnly;
            detail::bSummaryOnly = true;
            detail::ReportOnLayer(poLayer, nullptr, nullptr, nullptr,
              bListMDD, bShowMetadata,
              papszExtraMDDomains, bFeatureCount, bExtent);
            detail::bSummaryOnly = bSummaryOnlyBackup;
          }
          if (!detail::bSuperQuiet && !detail::bSummaryOnly)
            poFeature->DumpReadable(nullptr, detail::papszOptions);
        }
        OGRFeature::DestroyFeature(poFeature);
      }
      }

    /* -------------------------------------------------------------------- */
    /*      Special case for -sql clause.  No source layers required.       */
    /* -------------------------------------------------------------------- */
    else if (pszSQLStatement) {
      nRepeatCount = 0;  // skip layer reporting.

      if (CSLCount(papszLayers) > 0)
        printf("layer names ignored in combination with -sql.\n");

      OGRLayer* poResultSet =
        poDS->ExecuteSQL(
          pszSQLStatement,
          pszGeomField == nullptr ? poSpatialFilter : nullptr,
          pszDialect);

      if (poResultSet) {
        if (pszWHERE) {
          if (poResultSet->SetAttributeFilter(pszWHERE) != OGRERR_NONE)
          {
            printf("FAILURE: SetAttributeFilter(%s) failed.\n",
              pszWHERE);
            exit(1);
          }
        }

        if (pszGeomField != nullptr)
          detail::ReportOnLayer(poResultSet, nullptr, pszGeomField, poSpatialFilter,
            bListMDD, bShowMetadata, papszExtraMDDomains, bFeatureCount, bExtent);
        else
          detail::ReportOnLayer(poResultSet, nullptr, nullptr, nullptr,
            bListMDD, bShowMetadata, papszExtraMDDomains, bFeatureCount, bExtent);
        poDS->ReleaseResultSet(poResultSet);
      }
    }

    // coverity[tainted_data]
    for (int iRepeat = 0; iRepeat < nRepeatCount; iRepeat++) {
      if (papszLayers == nullptr || *papszLayers == nullptr) {
        if (iRepeat == 0)
          CPLDebug("OGR", "GetLayerCount() = %d\n", poDS->GetLayerCount());

        /* -------------------------------------------------------------------- */
        /*      Process each data source layer.                                 */
        /* -------------------------------------------------------------------- */
        for (int iLayer = 0; iLayer < poDS->GetLayerCount(); iLayer++) {
          OGRLayer* poLayer = poDS->GetLayer(iLayer);

          if (poLayer == nullptr) {
            printf("FAILURE: Couldn't fetch advertised layer %d!\n",
              iLayer);
            exit(1);
          }

          if (!bAllLayers) {
            printf("%d: %s", iLayer + 1, poLayer->GetName());

            const int nGeomFieldCount =
              poLayer->GetLayerDefn()->GetGeomFieldCount();
            if (nGeomFieldCount > 1) {
              printf(" (");
              for (int iGeom = 0; iGeom < nGeomFieldCount; iGeom++)
              {
                if (iGeom > 0)
                  printf(", ");
                OGRGeomFieldDefn* poGFldDefn =
                  poLayer->GetLayerDefn()->
                  GetGeomFieldDefn(iGeom);
                printf("%s", OGRGeometryTypeToName(poGFldDefn->GetType()));
              }
              printf(")");
            }
            else if (poLayer->GetGeomType() != wkbUnknown)
              printf(" (%s)", OGRGeometryTypeToName(poLayer->GetGeomType()));

            printf("\n");
          }
          else {
            if (iRepeat != 0)
              poLayer->ResetReading();

            detail::ReportOnLayer(poLayer, pszWHERE, pszGeomField, poSpatialFilter,
              bListMDD, bShowMetadata, papszExtraMDDomains, bFeatureCount, bExtent);
          }
        }
      }
      else {
        /* -------------------------------------------------------------------- */
        /*      Process specified data source layers.                           */
        /* -------------------------------------------------------------------- */

        for (char** papszIter = papszLayers; *papszIter != nullptr; ++papszIter) {
          OGRLayer* poLayer = poDS->GetLayerByName(*papszIter);

          if (poLayer == nullptr) {
            printf("FAILURE: Couldn't fetch requested layer %s!\n",
              *papszIter);
            exit(1);
          }

          if (iRepeat != 0)
            poLayer->ResetReading();

          detail::ReportOnLayer(poLayer, pszWHERE, pszGeomField, poSpatialFilter,
            bListMDD, bShowMetadata, papszExtraMDDomains,
            bFeatureCount, bExtent);
        }
      }
    }

    /* -------------------------------------------------------------------- */
    /*      Close down.                                                     */
    /* -------------------------------------------------------------------- */
    GDALClose(poDS);

#ifdef __AFL_HAVE_MANUAL_CONTROL
    }
#else
  end:
#endif

    CSLDestroy(argv);
    CSLDestroy(papszLayers);
    CSLDestroy(detail::papszOptions);
    CSLDestroy(papszOpenOptions);
    CSLDestroy(papszExtraMDDomains);
    if (poSpatialFilter)
      OGRGeometryFactory::destroyGeometry(poSpatialFilter);
    CPLFree(pszSQLStatement);
    CPLFree(pszWHERE);

    OGRCleanupAll();

    return nRet;
  }
}
