/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "sensormodel/utils/istereomodel.h"
#include "sensormodel/isensormodel.h"
#include "core/exceptions/exexcept.h"
#include "core/math/iintersect.h"
#include "core/spatialref/earthellipsoid.h"
#include "core/spatialref/groundpolicy.h"

#include <cassert>

namespace earthxpert {
class ISensorModel;

/**
 *  The mixin class StereoModel implements the template methods having logic
 *  common to mulitple sensor systems. A concrete sensor model may override
 *  the template method for better perforamnce. In detail, the bundle of
 *  rays from a block of sensor geometry models are used to implement
 *    forward intersection and backward projection in the network.
 *
 *  \tparam GroundPolicy defines the behaviour of and relationshp between
 *  the model coordinate system and the Cartesian coordinate system
 *
 *  \todo revist the design for variable number of rays, specifily,
 *  changing freaquently for matched key points: anohter class VariableStereoModel? 
 */
//template <class GroundPolicy>
class EXSENSORMODEL_EXPORTS StereoModel : public IStereoModel {
public:
  /**
   *  \brief Form a stereo model by a pair of sensor models.
   *
   *  \param [in] leftModel left sensor model composing the stereo model
   *  \param [in] rightModel right sensor model composing the stereo model
   */
  StereoModel(ISensorModel const* leftModel, ISensorModel const* rightModel);

  /**
   *  \brief Form a triplet stereo model by three sensor models.
   *
   *  \param [in] sensorModels triplet sensor models composing the stereo model
   */
  StereoModel(const std::array<ISensorModel const*, 3>& sensorModels);

  /**
   *  \brief form a stereo model by the given two or more sensor models.
   *
   *  \param [in] sensorModels multiple (two or more) sensor models composing
   * the stereo model.
   */
  StereoModel(const std::vector<ISensorModel const*>& sensorModels);

  StereoModel(const StereoModel&) = delete;
  StereoModel(StereoModel&&) = delete;
  virtual ~StereoModel() = default;

  StereoModel& operator=(const StereoModel&) = delete;
  StereoModel& operator=(StereoModel&&) = delete;

  void SetEarthEllipsoid(std::shared_ptr<EarthEllipsoid> earthEllip);

  const auto& GetSensorModels() const noexcept { return mModels; }

  /**
   *  clang-format off
   *  Image-to-ground backward projection (fixed).
   *
   *  \brief Given a pair of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] leftImage 2d position (x, y) in left image raster coordinate system
   *  \param [in] rightImage the corresponding 2d position (x, y) in right image
   *              raster coordinate system
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system 
   *  \return Returns true if the operation succeeds
   *  clang-format on
   */
  virtual bool ImageToGround(const Point2d &leftImage,
    const Point2d &rightImage, Point3d &ground) const override {
    assert(2 == mModels.size());
    switch (mFwdAlgo) {
    case eSM_CERES: {
      return ImageToGroundCeres(leftImage, rightImage, ground);
    }
    case eCA_INTERSECT: {
      Point2d images[2] = { leftImage, rightImage };
      return ImageToGroundX(images, 2, ground);
    }
    case eSM_NR: {
      return ImageToGroundNR(leftImage, rightImage, ground);
    }
    default: {
      return false;
    }
    }
  }

  /**
   *  \brief Given a pair of sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of pair-rays, and
   *  output the list of ground positions with respect to the ground coordinate
   * system.
   *
   *  \param [in] leftImages a list of 2d positions (x, y) in left image raster
   * coordinate system 
   *  \param [in] rightImages a list of corresponding 2d position
   * (x, y) in right image raster coordinate system 
   *  \param [in/out] grounds a
   * list of 3d ground positions (X, Y, Z) in ground coordinate system
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &leftImages,
    const std::vector<Point2d> &rightImages, std::vector<Point3d> &grounds) const override;

  /**
   *  \brief Given a triplet of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of triple-rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] image the corresponding 2d positions (x, y) in imagery raster
   * coordinate systems 
   *  \param [in/out] ground 3d ground position (X, Y, Z) in
   * ground coordinate system \return Returns true if the operation succeeds
   *  \return Returns true if the operation succeeds
   */
  virtual bool
  ImageToGround(const std::array<Point2d, 3> &images, Point3d &ground) const override {
    assert(3 == mModels.size());
    switch (mFwdAlgo) {
    case eSM_CERES: {
      return ImageToGroundCeres(images, ground);
    }
    case eCA_INTERSECT: {
      return ImageToGroundX(images.data(), 3, ground);
    }
    case eSM_NR: {
      return ImageToGroundNR(images, ground);
    }
    default: {
      return false;
    }
    }
  }

  /**
   *  \brief Given two or more sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. 
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  virtual bool
  ImageToGround(const std::vector<Point2d> &images, Point3d &ground) const override {
    const int numRays = int(mModels.size());
    assert(numRays == images.size());
    switch (mFwdAlgo) {
    case eSM_CERES: {
      return ImageToGroundCeres(images.data(), numRays, ground);
    }
    case eCA_INTERSECT: {
      return ImageToGroundX(images.data(), numRays, ground);
    }
    case eSM_NR: {
      //return ImageToGroundNR(images, numRays, ground);
      throw std::exception(_("StereoModel: not implemented"));
    }
    default: {
        return false;
    }
    }
  }

  /**
   *  Image-to-ground forward projection (variable).
   *
   *  \brief Given a variable number of sensor models and associated 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground position with respect to the ground coordinate system.
   *
   *  \param [in] sensorModels an array of sensor models.
   *  \param [in] images the corresponding 2d positions (x, y) in
   * imagery raster coordinate systems, interleaved by sensor models. 
   *  \param [in/out] ground 3d ground position (X, Y, Z) in ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  virtual bool ImageToGround(const std::vector<const ISensorModel*>& sensorModels,
    const std::vector<Point2d>& images, Point3d& ground) const override {
    return false;
  }

  /**
   *  \brief Given two or more sensor models and associated list of 2d positions
   *  in image coordinate system, do forward intersection of rays, and
   *  output the ground positions with respect to the ground coordinate system.
   *
   *  \param [in] images List of corresponding 2d positions (x,
   * y) in imagery raster coordinate systems, interleaved by sensor models.
   *  \param [in/out] grounds List of 3d ground positions (X, Y, Z) in ground
   * coordinate system
   */
  virtual size_t ImageToGround(const std::vector<Point2d> &images,
                std::vector<Point3d> &grounds) const override;

  /**
   *  Ground-to-image backward projection (fixed).
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   * system and multiple sensor models, output the corresponding 2d sensor
   * positions in the image coordinate system for each sensor model.
   *
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] images 2d position (x, y) in sensor raster
   * coordinate system \return Returns true if operation succeeds
   */
  virtual bool GroundToImage(const Point3d &ground,
                std::vector<Point2d> &images) const override;

  /**
   *  Ground-to-image backward projection (variable).
   *
   *  \brief Given a 3d ground position with reference to the ground coordinate
   *  system and a variable number of sensor models, output the corresponding 2d sensor
   *  positions in the image coordinate system for each sensor model.
   *
   *  \param [in] sensorModels an array of sensor models.
   *  \param [in] ground 3d position (X, Y, Z) in ground coordinate system
   *  \param [out] images 2d position (x, y) in imagery raster coordinate system 
   *  \return Returns true if operation succeeds
   */
  virtual bool GroundToImage(const std::vector<const ISensorModel*>& sensorModels,
    const Point3d& ground, std::vector<Point2d>& images) const {
    return false;
  }

  /**
   *  \brief Given a list of 3d positions in ground coordinate system and
   *  multiple sensor models, output a list of 2d sensor positions in image
   * coordinate system.
   *
   *  \param [in] ground List of 3d ground positions (X, Y, Z)
   *  \param [in/out] images List of 2d positions (x, y) on
   * image, interleaved by sensor models
   */
  virtual size_t GroundToImage(const std::vector<Point3d> &grounds,
                std::vector<Point2d> &images) const override;

  void SetForwardAlgorithm(const ForwardAlgo fwdAlgo);
  void SetOptions(const Solver::Options& options) { mOptions = options; }

  auto GetForwardAlgorithm() const noexcept { return mFwdAlgo; }
  auto& GetOptions() noexcept { return mOptions; }

protected:
  static constexpr int kMaxIters{ 8 };
  static constexpr double kTolZ{ 1.0e-4 };  // [mm]

  ForwardAlgo mFwdAlgo{ eSM_NR };
  Solver::Options mOptions;   // the options to control ceres solver

private:
  static constexpr double kH{ 10.0 };
  static constexpr double kInvH{ 1.0 / kH };

  std::vector<const ISensorModel*> mModels;   // one model per ray
  std::shared_ptr<EarthEllipsoid> mEarthEllip;
  std::unique_ptr<math::IIntersect> mIntersect;

  void Initialize();

  bool ImageToGroundCeres(const Point2d& leftImage, const Point2d& rightImage, Point3d& ground) const;
  bool ImageToGroundNR(const Point2d& leftImage, const Point2d& rightImage, Point3d& ground) const;

  bool ImageToGroundCeres(const std::array<Point2d, 3>& images, Point3d& ground) const;
  bool ImageToGroundNR(const std::array<Point2d, 3>& images, Point3d& ground) const;

  bool ImageToGroundCeres(const Point2d* images, int numRays, Point3d& ground) const;
  bool ImageToGroundX(const Point2d* images, int numRays, Point3d& ground) const;

  // cache
  SensorType mSensorType{ SensorType::eUNDEFINED };
  SrsType mSrsType{ eUNKNOWN };
};
} // namespace earthxpert
