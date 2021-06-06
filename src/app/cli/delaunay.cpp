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
    ("backend", bpo::value<std::string>()->required(), "engine: cv|gg|gts")
    ("lasf", bpo::value<std::string>()->required(), ".las file")
    ("tinf", bpo::value<std::string>()->required(), ".gts/.ply file")
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
 *      --backend     <cv|gg|gnu>              // "cv"
 *      --lasf        [path_of_las_file]    // ".las"
 *      --tinf        [path_of_gts_file]    // ".gts", ".ply"
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
  dlOpt.tinf = vm["tinf"].as<std::string>();
  dlOpt.memSize = size_t(vm["memsize"].as<int>()) * 1024 * 1024;  // [unit: bytes]

  fs::path filePath(dlOpt.lasf);

  if (std::string::npos != dlOpt.backend.find("cv")) {
    if (dlOpt.tinf.empty()) {
      dlOpt.tinf = filePath.parent_path().string() +
        utils::PathReplaceExt(dlOpt.lasf, ".ply");
    }
    CvOptions cvOpt;
    if (std::string::npos != dlOpt.tinf.find(".gts")) {
      Las2Gts(dlOpt.lasf, dlOpt.tinf, cvOpt);
    }
    else if (std::string::npos != dlOpt.tinf.find(".ply")) {
      Las2Ply(dlOpt.lasf, dlOpt.tinf, cvOpt);
    }
    else {
      assert(false && "unsupported format");
    }
  }    // OpenCV Subdiv2D
  else if (std::string::npos != dlOpt.backend.find("gg")) {
    GEO::initialize();
    if (dlOpt.tinf.empty()) {
      dlOpt.tinf = filePath.parent_path().string() +
        utils::PathReplaceExt(dlOpt.lasf, ".ply");
    }
    GeogramOptions ggOpt;
    if (std::string::npos != dlOpt.tinf.find(".gts")) {
      Las2Gts(dlOpt.lasf, dlOpt.tinf, ggOpt);
    }
    else if (std::string::npos != dlOpt.tinf.find(".ply")) {
      Las2Ply(dlOpt.lasf, dlOpt.tinf, ggOpt);
    }
    else {
      assert(false && "unsupported format");
    }
  }    // Geogram Delaunay2d
  else if (std::string::npos != dlOpt.backend.find("gts")) {
    if (dlOpt.tinf.empty()) {
      dlOpt.tinf = filePath.parent_path().string() +
        utils::PathReplaceExt(dlOpt.lasf, ".gts");
    }
    GtsOptions gtsOpt;
    if (std::string::npos != dlOpt.tinf.find(".gts")) {
      Las2Gts(dlOpt.lasf, dlOpt.tinf, gtsOpt);
    }
    else if (std::string::npos != dlOpt.tinf.find(".ply")) {
      Las2Ply(dlOpt.lasf, dlOpt.tinf, gtsOpt);
    }
    else {
      assert(false && "unsupported format");
    }
  }    // GNU Triangulated Surface
  else {
    assert(false && "backend is not supported");
  }

  return 0;
}
