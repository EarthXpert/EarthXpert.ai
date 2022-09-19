/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "3rdparty/opencv/exopencv.h"
#include "core/exceptions/exexcept.h"
#include "core/math/rotation.h"
#include <iostream>

using namespace earthxpert;
using namespace earthxpert::math;
using namespace::Eigen;


/**
 *
 */
TEST(rotation, euler) {
  const double angles[3] = {0.1, 0.2, 0.3};
  cv::Mat cvPt(Point3d(1, 1, 1));
  Vector3d eigenPt(1, 1, 1);

  // three axes
  cv::Mat1d cvRot{ cv::Mat1d::eye(3, 3) };
  EulerXYZToMatrix(angles, cvRot);  
  std::cout << cvRot << std::endl;
  const cv::Mat cvPtT = cvRot * cvPt;
  std::cout << cvPtT << std::endl;

  Matrix3d eigenRot;
  EulerXYZToMatrix(angles, eigenRot);
  std::cout << eigenRot << std::endl;
  const Vector3d eigenPtT = eigenRot * eigenPt;
  std::cout << eigenPtT << std::endl;

  // single axis
  for (int i = 0; i < 3; ++i) {
    double ang[3] = { 0 };
    ang[i] = angles[i];

    EulerXYZToMatrix(ang, cvRot);
    std::cout << cvRot << std::endl;
    std::cout << cvRot * cvPt << std::endl;

    EulerXYZToMatrix(ang, eigenRot);
    std::cout << eigenRot << std::endl;
    std::cout << eigenRot * eigenPt << std::endl;
  }
}
