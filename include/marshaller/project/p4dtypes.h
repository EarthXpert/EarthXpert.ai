/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"

#include <string>

namespace earthxpert::p4d {
/**
 *  \options .p4d
 */
enum KpImageScale {
  eFULL
};

struct P4dInitial {
  bool runStep{};
  KpImageScale keypointsImageScale{ eFULL };
  int imageScale{ 1 };
};

struct P4dDense {
  bool runStep{};
};

struct P4dMesh {
  bool meshGenerate3dTexturedMesh{};
};

struct P4dOrtho {
  bool runStep{};
  int orthoResolution{ -1 };
};

struct P4dIndex {

};

/**
 *  \inputs
 */
struct P4dBand {
  std::string name;
  int centralWaveLength;
  int width;
  double weight;
};

enum P4dLensType {
  eFisheye,
  ePerspective
};

struct P4dCamera {
  std::string name;
  int imageWidth{};
  int imageHeight{};
  double pixelSize{};
  double principalPointXmm{};
  double principalPointYmm{};
  P4dLensType lensType{ ePerspective };
  double focalLengthmm{};
  int distortion{ 5 };
  double radialK1{};
  double radialK2{};
  double radialK3{};
  double tangentialT1{};
  double tangentialT2{};
  std::string cameraModelSource;
  std::vector<P4dBand> bands;
  std::string serialNumber;
  bool fixedBands{};
  int id{};
};

struct P4dCS {
  std::string WKT;
  struct VerticalDef {
    std::string model;
    std::string geoidHeight;
  };
};

struct P4dCoordinateSystems {
  bool southingWesting{ true };
  P4dCS output;
  P4dCS gcp;
  P4dCS image;
};

enum P4dGcpType {
  e3DGCP
};

struct P4dGcp {
  std::string label;
  double alt{};
  double lat{};
  double lng{};
  double x{};
  double y{};
  double z{};
  bool tiePoint3D{false};
  std::string id;
  double accuracyXY{};
  double accuracyZ{};
  P4dGcpType type{ e3DGCP };
};

struct P4dImageGcp {
  float x{};
  float y{};
  std::string id;
  float scale{ 1 };
};

struct P4dImage {
  std::string path;
  std::string group;
  bool enabled{ true };

  struct {
    std::string name;
    std::string id;
  } camera; 
  std::string time;
  struct {
    double alt{};
    double lat{};
    double lng{};
  } gps;
  struct {
    double x{};
    double y{};
    double z{};
  } xyz;
  float accuracyXY{};
  float accuracyZ{};
  struct {
    double yaw;
    double pitch;
    double roll{};
    bool perpCamera{ true };
  } ori;
  struct {
    float yaw;
    float pitch;
    float roll{};
    bool perpCamera{ true };
  } accuracyOri;
  std::vector<P4dImageGcp> imageGcps;
};

/**
 *  \params
 */
// \1_initial\params\*_pix4d_calibrated_internal_camera_parameters.cam
struct P4dCameraCal {
  double width{};
  double height{};
  double F{};
  double Px{};
  double Py{};
  double K1{};
  double K2{};
  double K3{};
  double T1{};
  double T2{};
};

// \1_initial\params\*_calibrated_internal_camera_parameters.cam
struct CameraCal {

};

// *_tp_bingo.txt
struct BingoTp {
  int id{};
  float x{};  // [mm]
  float y{};
};

// *_tp_pix4d.txt
struct P4dTp {
  int id{};
  float x{};  // [pixel]
  float y{};
  float f{};  // zoom factor
};
}
