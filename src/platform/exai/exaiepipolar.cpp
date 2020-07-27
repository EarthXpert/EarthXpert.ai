/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaiepipolar.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/rasterresample.h"
#include "sensormodel/utils/utils.h"

#include <iostream>

using namespace earthxpert;

ExaiEpipolar::ExaiEpipolar() {
}

/**
 *  \brief 
 */
int ExaiEpipolar::Execute() {
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

  EpipolarOptions epiOpt;
  epiOpt.sensorName = vm["sensorname"].as<std::string>();
  const auto st = SensorNameToType(epiOpt.sensorName);
  if (SensorType::eRATIONAL != st && SensorType::eFRAME != st) {
    throw std::invalid_argument("Epipolar not implemented fro sensor: " + epiOpt.sensorName);
  }

  epiOpt.leftImgFile = vm["leftimg"].as<std::string>();
  epiOpt.nadImgFile = vm["nadimg"].as<std::string>();
  epiOpt.rightImgFile = vm["rightimg"].as<std::string>();
  epiOpt.bands = vm["bands"].as<std::vector<int>>();
  epiOpt.sample = vm["sample"].as<int>();
  epiOpt.stride = vm["stride"].as<int>();
  epiOpt.grey = vm["grey"].as<bool>();
  epiOpt.oneFile = vm["onefile"].as<bool>();
  epiOpt.outOpt = vm["outopt"].as<std::string>();
  epiOpt.outPath = vm["outdir"].as<std::string>();
  epiOpt.resample = vm["resample"].as<std::string>();
  epiOpt.memSize = size_t(vm["memsize"].as<int>()) * 1024 * 1024;  // [unit: bytes]

  switch (st) {
  case SensorType::eRATIONAL: {
    epiOpt.leftMdlFile = vm["leftmdl"].as<std::string>();
    epiOpt.nadMdlFile = vm["nadmdl"].as<std::string>();
    epiOpt.rightMdlFile = vm["rightmdl"].as<std::string>();

    //std::cout << ToString(&epiOpt);
    GenerateRationalEpipolar(epiOpt);
    break;
  }

  default:
    assert(false && "sensor is not supported");
  }

  return 0;
}

void ExaiEpipolar::AddFrameOptions(bpo::options_description& optDesc) {
}

/**
 *  \brief
 */
void ExaiEpipolar::AddRationalOptions(bpo::options_description & optDesc) {
    optDesc.add_options()
    ("sensorname", bpo::value<std::string>()->required(), "sensor name: frame|rational")
    ("prjdir", bpo::value<std::string>(), "project directory or file")
    ("leftimg", bpo::value<std::string>()->required(), "left/fore image file")
    ("leftmdl", bpo::value<std::string>()->required(), "left/fore model file")
    ("nadimg", bpo::value<std::string>()->default_value(""), "nadir image file")
    ("nadmdl", bpo::value<std::string>()->default_value(""), "nadir model file")
    ("rightimg", bpo::value<std::string>()->required(), "right/aft image file")
    ("rightmdl", bpo::value<std::string>()->required(), "right/aft model file")
    ("bands,b", bpo::value<std::vector<int>>()->multitoken(), "list of bands")
    ("sample,s", bpo::value<int>()->default_value(1), "downsample factor")
    ("stride,d", bpo::value<int>()->default_value(8), "stride interval")
    ("grey,g", bpo::value<bool>()->default_value(false), "convert to grey image")
    ("onefile,f", bpo::value<bool>()->default_value(false), "save two epipolar images in a single file")
    ("outopt", bpo::value<std::string>()->default_value("full"), "output option: compact|full|intersect")
    ("outdir,o", bpo::value<std::string>()->required(), "output directory")
    ("resample,r", bpo::value<std::string>()->default_value("cubic"), "resample algorithm: bilinear|cubic|gausian")
    ("memsize,m", bpo::value<int>()->default_value(4000), "memory size in MB")
    ("help,h", "Usage help");
}

void ExaiEpipolar::Help() {
  printf("\nUsage: exai epipolar\n");
}
