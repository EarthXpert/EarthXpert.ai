/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/sensormodelfactory.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/rationalmodel.h"

using earthxpert::DigitalFrameModel;
using earthxpert::FilmFrameModel;
using earthxpert::ISensorModel;
using earthxpert::RationalModel;
using earthxpert::SensorModelFactory;

/**
 *  \brief
 */
std::unique_ptr<ISensorModel> SensorModelFactory::CreateSensorModel() {
  return std::unique_ptr<ISensorModel>();
}

/**
 *  \brief
 */
std::unique_ptr<FilmFrameModel> SensorModelFactory::CreateFilmFrameModel() {
  return std::unique_ptr<FilmFrameModel>();
}

/**
 *  \brief
 */
std::unique_ptr<DigitalFrameModel>
SensorModelFactory::CreateDigitalFrameModel() {
  return std::unique_ptr<DigitalFrameModel>();
}

/**
 *  \brief
 */
std::unique_ptr<RationalModel> SensorModelFactory::CreateRationalModel() {
  return std::unique_ptr<RationalModel>();
}
