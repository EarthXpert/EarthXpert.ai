/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
/******************************************************************************
*
* Project:  GDAL Utilities
* Purpose:  Command line application to list info about a given CRS.
*           Outputs a number of formats (WKT, PROJ.4, etc.).
* Author:   Frank Warmerdam, warmerdam@pobox.com
*           Etienne Tourigny, etourigny.dev-at-gmail-dot-com
*
* ****************************************************************************
* Copyright (c) 1998, Frank Warmerdam
* Copyright (c) 2011-2013, Even Rouault <even dot rouault at mines-paris dot org>
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
#include "platform/gdal/gdalsrsinfo.h"

#include "commonutils.h"
#include "cpl_string.h"
#include "ogrsf_frmts.h"

CPL_CVSID("$Id: gdalsrsinfo.cpp 6574497e5ddfd7c08c094a76756a0ef477cef6a1 2018-04-04 22:15:20 +0200 Even Rouault $")

using namespace earthxpert;

/**
 *
 */
#define CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(nExtraArg) \
    do { if (i + nExtraArg >= mArgc) \
        Help(CPLSPrintf("%s option requires %d argument(s)", \
                         mArgv[i], nExtraArg)); } while( false )

int GdalSrsInfo::Execute() {
  bool bGotSRS = false;
  bool bPretty = false;
  bool bValidate = false;
  bool bFindEPSG = false;
  int  nEPSGCode = -1;
  const char* pszInput = nullptr;
  const char* pszOutputType = "default";
  OGRSpatialReference  oSRS;

  /* Check strict compilation and runtime library version as we use C++ API */
  if (!GDAL_CHECK_VERSION(mArgv[0]))
    return -1;

  EarlySetConfigOptions(mArgc, mArgv);

  /* -------------------------------------------------------------------- */
  /*      Register standard GDAL drivers, and process generic GDAL        */
  /*      command options.                                                */
  /* -------------------------------------------------------------------- */
  mArgc = GDALGeneralCmdLineProcessor(mArgc, &mArgv, 0);
  if (mArgc < 1)
    return -1;

  /* -------------------------------------------------------------------- */
  /*      Parse arguments.                                                */
  /* -------------------------------------------------------------------- */
  for (int i = 1; i < mArgc; i++) {
    CPLDebug("gdalsrsinfo", "got arg #%d : [%s]", i, mArgv[i]);

    if (EQUAL(mArgv[i], "--utility_version")) {
      printf("%s was compiled against GDAL %s and is running against GDAL %s\n",
        mArgv[0], GDAL_RELEASE_NAME, GDALVersionInfo("RELEASE_NAME"));
      CSLDestroy(mArgv);
      return 0;
    }
    else if (EQUAL(mArgv[i], "-h") || EQUAL(mArgv[i], "--help")) {
      Help();
      return 0;
    }
    else if (EQUAL(mArgv[i], "-e"))
      bFindEPSG = true;
    else if (EQUAL(mArgv[i], "-o")) {
      CHECK_HAS_ENOUGH_ADDITIONAL_ARGS(1);
      pszOutputType = mArgv[++i];
    }
    else if (EQUAL(mArgv[i], "-p"))
      bPretty = true;
    else if (EQUAL(mArgv[i], "-V"))
      bValidate = true;
    else if (mArgv[i][0] == '-') {
      Help(CPLSPrintf("Unknown option name '%s'", mArgv[i]));
      return 0;
    }
    else
      pszInput = mArgv[i];
  }

  if (!pszInput) {
    CSLDestroy(mArgv);
    Help("No input specified.");
    return 0;
  }

  /* Search for SRS */
  /* coverity[tainted_data] */
  bGotSRS = FindSRS(pszInput, oSRS) == TRUE;

  CPLDebug("gdalsrsinfo",
    "bGotSRS: %d bValidate: %d pszOutputType: %s bPretty: %d",
    static_cast<int>(bGotSRS),
    static_cast<int>(bValidate),
    pszOutputType,
    static_cast<int>(bPretty));

  /* Make sure we got a SRS */
  if (!bGotSRS) {
    CPLError(CE_Failure, CPLE_AppDefined,
      "ERROR - failed to load SRS definition from %s",
      pszInput);
  }

  else {
    int nEntries = 0;
    int* panConfidence = nullptr;
    OGRSpatialReferenceH* pahSRS = nullptr;

    /* Find EPSG code */
    if (EQUAL(pszOutputType, "epsg"))
      bFindEPSG = true;

    if (bFindEPSG) {
      pahSRS = OSRFindMatches(reinterpret_cast<OGRSpatialReferenceH>(
        const_cast<OGRSpatialReference*>(&oSRS)),
        nullptr, &nEntries, &panConfidence);
    }

    for (int i = 0; i < (nEntries ? nEntries : 1); i++) {
      if (nEntries) {
        oSRS = *reinterpret_cast<OGRSpatialReference*>(pahSRS[i]);
        if (panConfidence[i] != 100) {
          printf("Confidence in this match: %d %%\n",
            panConfidence[i]);
        }

        const char* pszAuthorityCode =
          oSRS.GetAuthorityCode(nullptr);
        if (pszAuthorityCode)
          nEPSGCode = atoi(pszAuthorityCode);
      }

      /* Validate - not well tested!*/
      if (bValidate) {
        OGRErr eErr = oSRS.Validate();
        if (eErr != OGRERR_NONE) {
          printf("\nValidate Fails");
          if (eErr == OGRERR_CORRUPT_DATA)
            printf(" - SRS is not well formed");
          else if (eErr == OGRERR_UNSUPPORTED_SRS)
            printf(" - contains non-standard PROJECTION[] values");
          printf("\n");
        }
        else
          printf("\nValidate Succeeds\n");
      }

      /* Output */
      if (EQUAL("default", pszOutputType)) {
        /* does this work in MSVC? */
        const char* papszOutputTypes[] =
        { "proj4", "wkt", nullptr };
        if (bFindEPSG)
          printf("\nEPSG:%d\n", nEPSGCode);
        PrintSRSOutputTypes(oSRS, papszOutputTypes);
      }
      else if (EQUAL("all", pszOutputType)) {
        if (bFindEPSG)
          printf("\nEPSG:%d\n\n", nEPSGCode);
        const char* papszOutputTypes[] = { "proj4","wkt","wkt_simple","wkt_noct","wkt_esri","mapinfo","xml",nullptr };
        PrintSRSOutputTypes(oSRS, papszOutputTypes);
      }
      else if (EQUAL("wkt_all", pszOutputType)) {
        const char* papszOutputTypes[] = { "wkt", "wkt_simple", "wkt_noct", "wkt_esri", nullptr };
        PrintSRSOutputTypes(oSRS, papszOutputTypes);
      }
      else {
        if (bPretty)
          printf("\n");
        if (EQUAL(pszOutputType, "epsg"))
          printf("EPSG:%d\n", nEPSGCode);
        else
          PrintSRS(oSRS, pszOutputType, bPretty, FALSE);
        if (bPretty)
          printf("\n");
      }
    }

    OSRFreeSRSArray(pahSRS);
    CPLFree(panConfidence);
  }

  /* cleanup anything left */
  GDALDestroyDriverManager();
  OGRCleanupAll();
  CSLDestroy(mArgv);

  return 0;
}

void GdalSrsInfo::Help(const char* pszErrorMsg) {
  printf("\nUsage: exgdal srsinfo [options] srs_def\n"
    "\n"
    "srs_def may be the filename of a dataset supported by GDAL/OGR "
    "from which to extract SRS information\n"
    "OR any of the usual GDAL/OGR forms "
    "(complete WKT, PROJ.4, EPSG:n or a file containing the SRS)\n"
    "\n"
    "Options: \n"
    "   [--help-general] [-h]  Show help and exit\n"
    "   [-p]                   Pretty-print where applicable (e.g. WKT)\n"
    "   [-V]                   Validate SRS\n"
    "   [-e]                   Search for EPSG number(s) corresponding to SRS\n"
    "   [-o out_type]          Output type { default, all, wkt_all,\n"
    "                                        proj4, epsg,\n"
    "                                        wkt, wkt_simple, wkt_noct, wkt_esri,\n"
    "                                        mapinfo, xml }\n\n");

  if (pszErrorMsg)
    fprintf(stderr, "\nFAILURE: %s\n", pszErrorMsg);
}

/**
 *  \brief Search for SRS from pszInput, update oSRS.
 */
bool GdalSrsInfo::FindSRS(const char* pszInput, OGRSpatialReference& oSRS) {
  bool bGotSRS = false;
  GDALDataset* poGDALDS = nullptr;
  OGRLayer* poLayer = nullptr;
  const char* pszProjection = nullptr;
  CPLErrorHandler oErrorHandler = nullptr;
  bool bIsFile = false;
  OGRErr eErr = OGRERR_NONE;

  /* temporarily suppress error messages we may get from xOpen() */
  bool bDebug = CPLTestBool(CPLGetConfigOption("CPL_DEBUG", "OFF"));
  if (!bDebug)
    oErrorHandler = CPLSetErrorHandler(CPLQuietErrorHandler);

  /* Test if argument is a file */
  VSILFILE* fp = VSIFOpenL(pszInput, "r");
  if (fp) {
    bIsFile = true;
    VSIFCloseL(fp);
    CPLDebug("gdalsrsinfo", "argument is a file");
  }

  /* try to open with GDAL */
  if (!STARTS_WITH(pszInput, "http://spatialreference.org/")) {
    CPLDebug("gdalsrsinfo", "trying to open with GDAL");
    poGDALDS = static_cast<GDALDataset*>(GDALOpenEx(pszInput, 0, nullptr, nullptr, nullptr));
  }
  if (poGDALDS) {
    pszProjection = poGDALDS->GetProjectionRef();
    if (pszProjection && pszProjection[0] != '\0') {
      if (oSRS.importFromWkt(pszProjection) == OGRERR_NONE) {
        CPLDebug("gdalsrsinfo", "got SRS from GDAL");
        bGotSRS = true;
      }
    }
    else if (poGDALDS->GetLayerCount() > 0) {
      poLayer = poGDALDS->GetLayer(0);
      if (poLayer) {
        OGRSpatialReference* poSRS = poLayer->GetSpatialRef();
        if (poSRS) {
          CPLDebug("gdalsrsinfo", "got SRS from OGR");
          bGotSRS = true;
          OGRSpatialReference* poSRSClone = poSRS->Clone();
          oSRS = *poSRSClone;
          OGRSpatialReference::DestroySpatialReference(poSRSClone);
        }
      }
    }
    GDALClose(poGDALDS);
    if (!bGotSRS)
      CPLDebug("gdalsrsinfo", "did not open with GDAL");
  }

  /* Try ESRI file */
  if (!bGotSRS && bIsFile && strstr(pszInput, ".prj")) {
    CPLDebug("gdalsrsinfo",
      "trying to get SRS from ESRI .prj file [%s]", pszInput);

    char** pszTemp;
    if (strstr(pszInput, "ESRI::"))
      pszTemp = CSLLoad(pszInput + 6);
    else
      pszTemp = CSLLoad(pszInput);

    if (pszTemp) {
      eErr = oSRS.importFromESRI(pszTemp);
      CSLDestroy(pszTemp);
    }
    else
      eErr = OGRERR_UNSUPPORTED_SRS;

    if (eErr != OGRERR_NONE) {
      CPLDebug("gdalsrsinfo", "did not get SRS from ESRI .prj file");
    }
    else {
      CPLDebug("gdalsrsinfo", "got SRS from ESRI .prj file");
      bGotSRS = true;
    }
  }

  /* Last resort, try OSRSetFromUserInput() */
  if (!bGotSRS) {
    CPLDebug("gdalsrsinfo",
      "trying to get SRS from user input [%s]", pszInput);

    if (!CPLGetConfigOption("CPL_ALLOW_VSISTDIN", nullptr))
      CPLSetConfigOption("CPL_ALLOW_VSISTDIN", "YES");

    eErr = oSRS.SetFromUserInput(pszInput);

    if (eErr != OGRERR_NONE) {
      CPLDebug("gdalsrsinfo", "did not get SRS from user input");
    }
    else {
      CPLDebug("gdalsrsinfo", "got SRS from user input");
      bGotSRS = true;
    }
  }

  /* restore error messages */
  if (!bDebug)
    CPLSetErrorHandler(oErrorHandler);

  return bGotSRS;
}

/**
 *  \brief Print spatial reference in specified format
 */
CPLErr GdalSrsInfo::PrintSRS(const OGRSpatialReference& oSRS,
  const char* pszOutputType, bool bPretty, bool bPrintSep) {
  if (!pszOutputType || EQUAL(pszOutputType, ""))
    return CE_None;

  CPLDebug("gdalsrsinfo", "PrintSRS( oSRS, %s, %d, %d )\n",
    pszOutputType, static_cast<int>(bPretty),
    static_cast<int>(bPrintSep));

  char* pszOutput = nullptr;

  if (EQUAL("proj4", pszOutputType)) {
    if (bPrintSep) printf("PROJ.4 : ");
    oSRS.exportToProj4(&pszOutput);
    printf("%s\n", pszOutput);
  }

  else if (EQUAL("wkt", pszOutputType)) {
    if (bPrintSep) printf("OGC WKT :\n");
    if (bPretty)
      oSRS.exportToPrettyWkt(&pszOutput, FALSE);
    else
      oSRS.exportToWkt(&pszOutput);
    printf("%s\n", pszOutput);
  }

  else if (EQUAL("wkt_simple", pszOutputType)) {
    if (bPrintSep) printf("OGC WKT (simple) :\n");
    oSRS.exportToPrettyWkt(&pszOutput, TRUE);
    printf("%s\n", pszOutput);
  }

  else if (EQUAL("wkt_noct", pszOutputType)) {
    if (bPrintSep) printf("OGC WKT (no CT) :\n");
    OGRSpatialReference* poSRS = oSRS.Clone();
    poSRS->StripCTParms();
    if (bPretty)
      poSRS->exportToPrettyWkt(&pszOutput, FALSE);
    else
      poSRS->exportToWkt(&pszOutput);
    OGRSpatialReference::DestroySpatialReference(poSRS);
    printf("%s\n", pszOutput);
  }

  else if (EQUAL("wkt_esri", pszOutputType)) {
    if (bPrintSep) printf("ESRI WKT :\n");
    OGRSpatialReference* poSRS = oSRS.Clone();
    poSRS->morphToESRI();
    if (bPretty)
      poSRS->exportToPrettyWkt(&pszOutput, FALSE);
    else
      poSRS->exportToWkt(&pszOutput);
    OGRSpatialReference::DestroySpatialReference(poSRS);
    printf("%s\n", pszOutput);
  }

  else if (EQUAL("mapinfo", pszOutputType)) {
    if (bPrintSep) printf("MAPINFO : ");
    oSRS.exportToMICoordSys(&pszOutput);
    printf("\'%s\'\n", pszOutput);
  }

  else if (EQUAL("xml", pszOutputType)) {
    if (bPrintSep) printf("XML :\n");
    oSRS.exportToXML(&pszOutput, nullptr);
    printf("%s\n", pszOutput);
  }

  else {
    CPLError(CE_Failure, CPLE_AppDefined,
      "ERROR - %s output not supported",
      pszOutputType);
    return CE_Failure;
  }

  CPLFree(pszOutput);

  return CE_None;
}

/**
 *  \brief Print spatial reference in specified formats
 */
void GdalSrsInfo::PrintSRSOutputTypes(const OGRSpatialReference& oSRS,
  const char* const* papszOutputTypes) {
  int nOutputTypes = CSLCount(papszOutputTypes);
  printf("\n");
  for (int i = 0; i < nOutputTypes; i++) {
    PrintSRS(oSRS, papszOutputTypes[i], true, true);
    printf("\n");
  }
}
