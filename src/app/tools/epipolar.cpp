/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rd_party/boost/wxboost.h"
#include "3rd_party/gdal/wxgdal.h"
#include "core/exceptions/wxexcept.h"
#include "core/utils/rasterresample.h"
#include "rectification/epipolar/epipolar.h"
#include "sensormodel/utils.h"

#include <boost/program_options.hpp>
#include <iostream>

using namespace earthxpert;
using namespace earthxpert::epipolar;
using SensorType = ISensorModel::ModelType;
namespace po = boost::program_options;

/**
 *  The console program epipolar generates epipolar or pseudo epipolar images 
 *  from a pair of overlapping images acquired by the same sensor.
 *
 *  \detail Epipolar geometry is from the geometry of stereo vision. When a camera
 *  views a scene from two or more distinct positions, there are a number of 
 *  geometric relations between the 3D surface and its projections onto the 
 *  2D images that lead to constraints between the image pixels.
 *
 *  usage:  epipolar.exe 
 *            --sensorname <frame|rational>
 *            --prjdir [path_of_project_directory_or_file]
 *            --leftimg <path_of_left_image_file>    // "images/img01.tif"
 *            --leftmdl <path_of_left_model_file>    // "images/img01_rpc.tif"
 *            --rightimg <path_of_right_image_file>  // "images/img02.tif"
 *            --rightmdl <path_of_right_model_file>  // "images/img02_rpc.tif"
 *            --bands [list_of_band_ids]             // "1,2,3"
 *            --sample [downsample_factor]           // 2
 *            --outdir [path_of_output_directory]    // "d:/out/"
 *            --resample [bilinear|cubic|gaussian|sinc]
 *            --memsize [memory_size_in_MB]          // 4000
 *
 *  example:
 *
 *    wxepipolar.exe --sensorname="rational" --leftimg="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB" --leftmdl="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/po_162796_pan_0000000_rpc.txt" --rightimg="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.TIF" --rightmdl="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB" --bands=1 --sample=1 --outdir="D:/home/workspace/build/windows/vs2019/tests/output/" --resample="cubic" --memsize=4000
 *    wxepipolar.exe --sensorname="rational" --leftimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000.tif" --leftmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt" --rightimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000.tif" --rightmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt" --bands=1 --sample=1 --outdir="D:/home/workspace/build/windows/vs2019/tests/output/" --resample="cubic" --memsize=4000
 */
int main(int argc, char** argv) {
  po::options_description optDesc{ "Options" };
  optDesc.add_options()
    ("sensorname,t", po::value<std::string>()->required(), "sensor name: frame|rational")
    ("prjdir,p", po::value<std::string>(), "project directory or file")
    ("leftimg,i", po::value<std::string>()->required(), "left image file")
    ("leftmdl,m", po::value<std::string>(), "left model file")
    ("rightimg,j", po::value<std::string>()->required(), "right image file")
    ("rightmdl,n", po::value<std::string>(), "right model file")
    ("bands,b", po::value<std::vector<int>>()->multitoken(), "list of bands")
    ("sample,s", po::value<int>()->default_value(1), "downsample factor")
    ("outdir,o", po::value<std::string>()->required(), "output directory")
    ("resample,r", po::value<std::string>()->default_value("cubic"), "resample algorithm: bilinear|cubic|gausian")
    ("memsize,e", po::value<int>()->default_value(4000), "memory size in MB")
    ("help,h", "Usage help");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, optDesc), vm);

  if (vm.count("help")) {
    std::cout << optDesc << std::endl;
    return 0;
  }

  try {
    po::notify(vm);
  }
  catch (...) {
    std::cout << optDesc << std::endl;
    return 0;
  }

  auto sn = vm["sensorname"].as<std::string>();
  const auto st = SensorNameToType(sn);
  if (SensorType::eRATIONAL != st && SensorType::eFRAME != st) {
    throw std::invalid_argument("Epipolar not implemented fro sensor: " + sn);
  }

  std::string leftImgFile = vm["leftimg"].as<std::string>();
  std::string leftMdlFile;
  std::string rightImgFile = vm["rightimg"].as<std::string>();
  std::string rightMdlFile;

  std::vector<int> bands = vm["bands"].as<std::vector<int>>();
  int sample = vm["sample"].as<int>();
  std::string outPath = vm["outdir"].as<std::string>();
  std::string resample = vm["resample"].as<std::string>();
  size_t memSize = size_t(vm["memsize"].as<int>()) * 1000;  // [unit: bytes]

  GDALInitialize();   // register GDAL drivers

  switch (st)
  {
  case SensorType::eRATIONAL:
  {
    leftMdlFile = vm["leftmdl"].as<std::string>();
    rightMdlFile = vm["rightmdl"].as<std::string>();

    GenerateRationalEpipolar(leftImgFile, leftMdlFile,
      rightImgFile, rightMdlFile, bands, sample, outPath, resample, memSize);
    break;
  }

  default:
    assert(false && "sensor is not supported");
  }

  return 0;
}
