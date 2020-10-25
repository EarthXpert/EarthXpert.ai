/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "srcconfig.h"
#include "3rdparty/boost/exboost.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "core/math/constants.h"
#include "core/math/rotation.h"
#include "core/utils/stringutils.h"
#include "core/utils/typehelper.h"

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "opencv2/core/mat.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace earthxpert;
using namespace earthxpert::math;

int main(int argc, char** argv) {
  // get full path of command line app by name
  std::string exe_path = SdkConfig::BinPath("exgeoinfo");
  std::cerr << "Full path: " << exe_path << std::endl;

  /* call EXAI's APIs */

  // rotation
  double angles[] = { -7.44063, 0.35937, 35.81236,
    -2.67784,50.27616,34.5366,
    -2.29608,40.32467,35.14538,
    -2.0846,30.29691,35.60032,
    -1.96875,20.34026,35.96587,
    -1.92484,10.32019,36.29562,
    -1.93181,0.3592,36.59673,
    -2.00497, -9.6306,36.90825,
    -2.13392, -19.62062,37.24114,
    -2.34715, -29.61001,37.63367,
    -2.67376, -39.59939,38.13304,
    -3.19971, -49.58497,38.82581 };

  for (int k = 0; k < 36; k+=3) {
    double* ang = angles + k;
    //std::cout << ang[0] << ", " << ang[1] << ", " << ang[2] << ";  \t";

    for (int i = 0; i < 3; ++i) {
      ang[i] *= kDEG2RAD;
    }

    cv::Mat1d rot{ cv::Mat1d::eye(3, 3) };
    double att[3]{};

    EulerToMatrix<eXYZ>(ang, rot);
    MatrixToEuler<eXYZ>(rot.t(), att);
    std::cout << Rad2Deg(att[0]) << ", " << Rad2Deg(att[1]) << ", " << Rad2Deg(att[2]) << ";  \t";

    EulerToMatrix<eYXZ>(ang, rot);
    MatrixToEuler<eXYZ>(rot.t(), att);
    std::cout << Rad2Deg(att[0]) << ", " << Rad2Deg(att[1]) << ", " << Rad2Deg(att[2]) << ";  \t";

    EulerToMatrix<eZXY>(ang, rot);
    MatrixToEuler<eXYZ>(rot.t(), att);
    std::cout << Rad2Deg(att[0]) << ", " << Rad2Deg(att[1]) << ", " << Rad2Deg(att[2]) << ";  \t";

    EulerToMatrix<eZYX>(ang, rot);
    MatrixToEuler<eXYZ>(rot.t(), att);
    std::cout << Rad2Deg(att[0]) << ", " << Rad2Deg(att[1]) << ", " << Rad2Deg(att[2]) << std::endl;
  }
  /*
  std::string eoTxt = "D:/Workspace/Downloads/Misc/PCI/Selim/EO_file2_UTM11_roll_pitch_yaw.txt";
  std::ifstream ifs(eoTxt.c_str());

  std::string line;
  for (int k = 0; k < 75514; ++k) {   // 
    std::getline(ifs, line);
    boost::trim(line);
    if (line.empty()) continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" =;,\t\n"),
      boost::algorithm::token_compress_on);

    if (tokens.empty() || 7 != tokens.size())
      throw std::exception(_("Each line contains 7 tokens"));

    double att[3] = { std::stod(tokens[4]), std::stod(tokens[5]), std::stod(tokens[6]) };
    //std::cout << att[0] << ", " << att[1] << ", " << att[2] << ";  \t";

    for (int i = 0; i < 3; ++i) {
      att[i] *= kDEG2RAD;
    }

    cv::Mat1d rot{ cv::Mat1d::eye(3, 3) };
    double attN[3]{};
    EulerToMatrix<eZXY>(att, rot);
    MatrixToEuler<eXYZ>(rot.t(), attN);
    std::cout << Rad2Deg(attN[0]) << ", " << Rad2Deg(attN[1]) << ", " << Rad2Deg(attN[2]) << std::endl;
  }*/
  return 0;
}
