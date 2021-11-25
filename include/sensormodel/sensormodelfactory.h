/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/framemodel.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/rationalmodel.h"

namespace earthxpert {
// 
constexpr int kNUMINTI = 20;  // number of intervals in image
constexpr int kNUMINTL = 10;  // number of layers in ground
constexpr int kMARGIN = 100;  // 

/**
 *  The class SensorModelFactory defines the behaviours for creating
 *  sensor model from image products.
 */
class EXSENSORMODEL_EXPORTS SensorModelFactory {
public:
  /**
   *  \brief
   */
  static std::unique_ptr<ISensorModel> CreateSensorModel();

  /**
   *  \brief
   */
  static std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel();

  /**
   *  \brief
   */
  static std::unique_ptr<FilmFrameModel> CreateFilmFrameModel();

  /**
   *  \brief create rational model from another sensor model
   */
  static std::unique_ptr<RationalModel> 
  CreateRationalModel(const ISensorModel* sensorModel, int numInt=kNUMINTI, 
    int numLyr=kNUMINTL, int margin=kMARGIN, bool cc=false);
};
} // namespace earthxpert
