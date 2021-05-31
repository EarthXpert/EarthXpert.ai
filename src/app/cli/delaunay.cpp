/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/pcutils.h"
#include "core/utils/fsutils.h"

#include <boost/program_options.hpp>
#include <iostream>

using namespace earthxpert;
namespace bpo = boost::program_options;

void AddOptions(bpo::options_description& optDesc) {
  optDesc.add_options()
    ("backend", bpo::value<std::string>()->required(), "engine: cv|gts")
    ("lasf", bpo::value<std::string>()->required(), ".las file")
    ("gtsf", bpo::value<std::string>()->required(), ".gts file")
    ("memsize,m", bpo::value<int>()->default_value(4000), "memory size in MB")
    ("help,h", "Usage help");
}

/**
 *  The console program exlas2gts triangulates a point cloud and writes
 *  the TIN into a text file in .gts format.
 *
 *  \detail 
 *
 *  usage:  las2gts.exe [options]
 *      --backend     <cv|gnu>              // "cv"
 *      --lasf        [path_of_las_file]    // ".las"
 *      --gtsf        [path_of_gts_file]    // ".gts"
 *      --memsize     [memory_size_in_MB]   // 4000
 *
 *  examples:
 *
 *    exlas2gts.exe 
 */
int main(int argc, char** argv) {
  bpo::options_description optDesc{ "Options" };
  AddOptions(optDesc);

  bpo::variables_map vm;
  bpo::store(bpo::parse_command_line(argc, argv, optDesc), vm);

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

  DelaunayOptions dlOpt;
  dlOpt.backend = vm["backend"].as<std::string>();
  dlOpt.lasf = vm["lasf"].as<std::string>();
  dlOpt.tinf = vm["gtsf"].as<std::string>();
  dlOpt.memSize = size_t(vm["memsize"].as<int>()) * 1024 * 1024;  // [unit: bytes]

  if (dlOpt.tinf.empty()) {
    fs::path filePath(dlOpt.lasf);
    dlOpt.tinf = filePath.parent_path().string() + 
      utils::PathReplaceExt(dlOpt.lasf, ".ply");
  }

  if (std::string::npos != dlOpt.backend.find("cv")) {
    CvOptions cvOpt;
    Las2Ply(dlOpt.lasf, dlOpt.tinf, cvOpt);
  }    // OpenCV Subdiv2D
  else if (std::string::npos != dlOpt.backend.find("gts")) {
    GtsOptions gtsOpt;
    Las2Gts(dlOpt.lasf, dlOpt.tinf, gtsOpt);
  }    // GNU Triangulated Surface
  else {
    assert(false && "backend is not supported");
  }

  return 0;
}
