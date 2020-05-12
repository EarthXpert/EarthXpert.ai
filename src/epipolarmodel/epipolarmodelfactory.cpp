/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "epipolarmodel/epipolarmodelfactory.h"
#include "core/utils/typehelper.h"
#include "epipolarmodel/epipolarmodel.h"
#include "sensormodel/framemodel.h"
#include "sensormodel/isensormodel.h"
#include "sensormodel/rationalmodel.h"

#include <cassert>
#include <cmath>
#include <exception>

using watxtract::EpipolarModelFactory;
using watxtract::FrameModel;
using watxtract::IEpipolarModel;
using watxtract::ISensorModel;
using watxtract::Point2d;
using watxtract::Point3d;
using watxtract::RationalModel;

/**
 *  \brief
 */
EpipolarModelFactory::EpipolarModelFactory(const EpipolarOption epiOpt)
  : mEpiOpt(epiOpt) {}

/**
 *  \brief Create an epipolar model from two sensor models composing of a stereo pair.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] height the average height used for epipolar rectficiation
 *  \return Returns pointer to epipolar model object.
 *  \remark This implementation requires that the stereo pair are of the same sensro type.
 */
std::unique_ptr<IEpipolarModel> EpipolarModelFactory::CreateEpipolarModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, 
  const int sample, const double height) {
  mSensorType = leftModel->GetModelType();
  if (mSensorType != rightModel->GetModelType())
    throw std::exception("The stereo pair should be of the same sensor type.");

  switch (mSensorType)
  {
  case SenType::eFRAME:
    return CreateFrameEpiModel(leftModel, rightModel, sample, height);
  case SenType::eRATIONAL:
    return CreateRationalEpiModel(leftModel, rightModel, sample);
  default:
    return nullptr; // model types not supported 
  }
}

/**
 *  \brief
 */
std::unique_ptr<IEpipolarModel> EpipolarModelFactory::CreateEpipolarModel(
  const std::array<std::shared_ptr<ISensorModel>, 2> sensorModels,
  const int sample, const double height) {
  return std::unique_ptr<IEpipolarModel>();
}

/**
 *  \brief
 */
std::unique_ptr<IEpipolarModel> EpipolarModelFactory::CreateEpipolarModel(
  const std::vector<std::shared_ptr<ISensorModel>> &sensorModels,
  const int sample, const double height) {
  if (sensorModels.size() < 2) {
    throw std::exception("Require a minimum of two images");
  }
  return std::unique_ptr<IEpipolarModel>();
}

/**
 *  \brief Create an epipolar model for a stereo pair of frame images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \param [in] height the optional average height used for epipolar rectficiation
 *  \return Returns pointer to epipolar model object.
 */
std::unique_ptr<IEpipolarModel> EpipolarModelFactory::CreateFrameEpiModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, 
  const int sample, const double height) {
    return nullptr;
  }

/**
 *  \brief Create an epipolar model for a stereo pair of RPC images.
 *
 *  \param [in] leftModel sensor model associated with the left image
 *  \param [in] rightModel sensor model associated with the right image
 *  \return Returns pointer to epipolar model object.
 */
std::unique_ptr<IEpipolarModel> EpipolarModelFactory::CreateRationalEpiModel(
  std::shared_ptr<ISensorModel> leftModel, 
  std::shared_ptr<ISensorModel> rightModel, const int sample) {
  using rnf = RationalModel::NormalizationFactor;

  auto leftRM = dynamic_cast<const RationalModel*>(leftModel.get());
  auto rightRM = dynamic_cast<const RationalModel*>(rightModel.get());

  auto lnfc = leftRM->GetNormalFators();
  auto rnfc = rightRM->GetNormalFators();

  // \todo across anti-meridian; unit literal
  const double LonOrig = Deg2Rad((lnfc[rnf::eOLONG] + rnfc[rnf::eOLONG]) / 2);
  const double LatOrig = Deg2Rad((lnfc[rnf::eOLAT] + rnfc[rnf::eOLAT]) / 2);
  const Point3d origin(LonOrig, LatOrig, 0);

  auto earthEllip = std::make_unique<EarthEllipsoid>();
  earthEllip->SetLsrOrigin(origin);

  const double height{ (lnfc[rnf::eOHEIGHT] + rnfc[rnf::eOHEIGHT]) / 2 };
  double xOffLeft{};
  double xOffRight{};
  double yOffset{};
  double scale{1.0};
  double rotation{};
  int numColsLeft{100};
  int numColsRight{100};
  int numRows{100};


  auto leftRect = std::make_unique<EpipolarRect>(origin, height, 
    xOffLeft, yOffset, scale, rotation, numColsLeft, numRows);

  auto rightRect = std::make_unique<EpipolarRect>(origin, height,
    xOffRight, yOffset, scale, rotation, numColsRight, numRows);

  auto geoGround = std::make_unique<GeographicGround>(std::move(earthEllip));
  assert(!earthEllip);

  return std::make_unique<GeoEpipolarModel>(leftModel, std::move(leftRect),
    rightModel, std::move(rightRect), std::move(geoGround));
}
