/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"

#include <string>

namespace earthxpert {
//
enum ExPointType {
  eUNUSED = -1,
  eGCP,   // ground control point
  eICP,   // independent check point
  eATP,   // automatic tie point
  eMTP    // manual tie point
};

// GroundPoint represents a multi-ray observation
struct GroundPoint {
  std::string id;
  Point3d point;      // XYZ, LPH position
};

struct ControlPoint : GroundPoint {
  ExPointType pt{ eGCP };
};

struct TiePoint : GroundPoint {
  ExPointType pt{ eATP };
};

struct ImagePoint {
  Point2f ip;        //  image position: col, row
  GroundPoint* gp;    // GroundPoint
  bool enabled{ true };   // flag indicating if used
};

// camera infomation
struct SensorInfo {
  std::string name;
  double fl;    // focal length/principal distance
  Point2d pp;   // PPA
};

struct ExImage {
  std::string sensor;   // camera
  std::string name;
  std::string path;   // path relative to the .epj
  bool enabled{ true };   // flag indicating if used

  std::vector<ImagePoint> mIPs;
};
}
