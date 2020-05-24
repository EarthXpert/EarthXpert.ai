/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/framemodel.h"

#include <memory>

namespace earthxpert {
class ISensorModel;
class RationalModel;

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
  static std::unique_ptr<FilmFrameModel> CreateFilmFrameModel();

  /**
   *  \brief
   */
  static std::unique_ptr<DigitalFrameModel> CreateDigitalFrameModel();

  /**
   *  \brief
   */
  static std::unique_ptr<RationalModel> CreateRationalModel();
};
} // namespace earthxpert
