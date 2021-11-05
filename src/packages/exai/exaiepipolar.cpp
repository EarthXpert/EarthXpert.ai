/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/exai/exaiepipolar.h"
#include "3rdparty/gdal/exgdal.h"
#include "3rdparty/utils/constants.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/rasterresample.h"
#include "sensormodel/utils/smutils.h"

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
  if (SensorType::eMT_RATIONAL != st && SensorType::eMT_FRAME != st) 
    throw std::invalid_argument(_("Epipolar not implemented "
      "for sensor type: " + epiOpt.sensorName));

  epiOpt.leftImgFile = vm["leftimg"].as<std::string>();
  epiOpt.nadImgFile = vm["nadimg"].as<std::string>();
  epiOpt.rightImgFile = vm["rightimg"].as<std::string>();
  epiOpt.bands = vm["bands"].as<std::vector<int>>();
  epiOpt.epiOpt = vm["epiopt"].as<std::string>();
  epiOpt.sample = vm["sample"].as<int>();
  epiOpt.stride = vm["stride"].as<int>();
  epiOpt.epiHgt = vm["epihgt"].as<float>();
  epiOpt.rotAngle = Deg2Rad(vm["rotangle"].as<float>());
  epiOpt.resample = vm["resample"].as<std::string>();
  epiOpt.outPath = vm["outdir"].as<std::string>();
  epiOpt.oneFile = vm["onefile"].as<bool>();
  epiOpt.grey = vm["grey"].as<bool>();
  epiOpt.outBkgd = vm["outbkgd"].as<int>();
  epiOpt.memSize = size_t(vm["memsize"].as<int>()) * kMB;  // [unit: bytes]

  switch (st) {
  case SensorType::eMT_RATIONAL: {
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

void ExaiEpipolar::AddOptions(bpo::options_description& optDesc) {

}

void ExaiEpipolar::AddFrameOptions(bpo::options_description& optDesc) {
}

/**
 *  \brief setup cli parameters
 *  \todo auto find model files.
 */
void ExaiEpipolar::AddRationalOptions(bpo::options_description& optDesc) {
    optDesc.add_options()
    ("sensorname", bpo::value<std::string>()->required(), "sensor name: frame|rational")
    ("prjdir", bpo::value<std::string>(), "project directory or file")
    //("images,i", bpo::value<std::string>()->multitoken(), "list of 2 or 3 images")
    ("leftimg", bpo::value<std::string>()->required(), "left/fore image file")
    ("leftmdl", bpo::value<std::string>()->default_value(""), "left/fore model file")
    ("nadimg", bpo::value<std::string>()->default_value(""), "nadir image file")
    ("nadmdl", bpo::value<std::string>()->default_value(""), "nadir model file")
    ("rightimg", bpo::value<std::string>()->required(), "right/aft image file")
    ("rightmdl", bpo::value<std::string>()->default_value(""), "right/aft model file")
    ("bands,b", bpo::value<std::vector<int>>()->multitoken(), "list of bands")
    ("epiopt", bpo::value<std::string>()->default_value("full"), "epipolar option: compact|full|intersect")
    ("sample,s", bpo::value<int>()->default_value(1), "downsample factor")
    ("stride,d", bpo::value<int>()->default_value(8), "stride interval")
    ("epihgt,h", bpo::value<float>()->default_value(0), "height of rectification plane")
    ("rotangle", bpo::value<float>()->default_value(0), "rotation angle of epipolar direction [degree]")
    ("resample,r", bpo::value<std::string>()->default_value("cubic"), "resample algorithm: bilinear|cubic|gausian")
    ("outdir,o", bpo::value<std::string>()->required(), "output directory")
    ("onefile,f", bpo::value<bool>()->default_value(false), "save two epipolar images in a single file")
    ("grey,g", bpo::value<bool>()->default_value(false), "convert to grey image")
    ("outbkgd", bpo::value<int>()->default_value(0), "output pixel background value")
    ("memsize,m", bpo::value<int>()->default_value(4000), "memory size [MB]")
    ("help,h", "Usage help");
}

void ExaiEpipolar::Help() {
  printf("\nUsage: exai epipolar\n");
}

int ExaiEpipolar::ExecuteImp() {
  return {};
}
