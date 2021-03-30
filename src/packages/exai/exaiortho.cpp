/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/exai/exaiortho.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/rasterresample.h"
#include "sensormodel/utils/smutils.h"

#include <iostream>

using namespace earthxpert;

ExaiOrtho::ExaiOrtho() {
}

/**
 *  \brief 
 */
int ExaiOrtho::Execute() {
  bpo::options_description optDesc{ "Options" };
  AddRationalOptions(optDesc);

  bpo::variables_map vm;
  bpo::store(bpo::parse_command_line(mArgc, mArgv, optDesc), vm);

  if (vm.count("help")) {
    std::cout << optDesc << std::endl;
    return 0;
  }

  try {
    bpo::notify(vm);
  }
  catch (...) {
    std::cout << optDesc << std::endl;
    return 0;
  }

  return 0;
}

void ExaiOrtho::AddFrameOptions(bpo::options_description& optDesc) {
}

/**
 *  \brief setup cli parameters
 *  \todo auto find model files.
 */
void ExaiOrtho::AddRationalOptions(bpo::options_description & optDesc) {
    optDesc.add_options()
    ("sensorname", bpo::value<std::string>()->required(), "sensor name: frame|rational")
    ("prjdir", bpo::value<std::string>(), "project directory or file")
    ("image", bpo::value<std::string>()->required(), "image file")
    ("model", bpo::value<std::string>()->required(), "model file")
    ("bands,b", bpo::value<std::vector<int>>()->multitoken(), "list of bands")
    ("dem", bpo::value<std::string>(), "DEM file | constant height")
    ("stride,d", bpo::value<int>()->default_value(8), "stride interval")
    ("rotangle", bpo::value<float>()->default_value(0), "rotation angle of ortho direction")
    ("resample,r", bpo::value<std::string>()->default_value("cubic"), "resample algorithm: bilinear|cubic|gausian")
    ("outdir,o", bpo::value<std::string>()->required(), "output directory")
    ("outsrs", bpo::value<std::string>(), "EPSG code | WKT")
    ("ulpos,p", bpo::value<std::vector<double>>()->multitoken(), "UL corner")
    ("outgsd,g", bpo::value<std::vector<double>>()->multitoken(), "GSD of ortho")
    ("outbkgd", bpo::value<int>()->default_value(0), "output pixel background value")
    ("memsize,m", bpo::value<int>()->default_value(4000), "memory size in MB")
    ("help,h", "Usage help");
}

void ExaiOrtho::Help() {
  printf("\nUsage: exai ortho\n");
}
