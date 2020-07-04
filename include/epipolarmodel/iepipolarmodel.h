/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/opencv/exopencv.h"
#include "epipolarmodel/exepipolarmodel.h"

#include <vector>

namespace earthxpert {
class ISensorModel;

/**
 *  The interface IEpipolarModel defines the geometric transform
 *  between the epipolar image and the associated image product
 *  used to generate it.
 */
class EXEPIPOLARMODEL_EXPORTS IEpipolarModel {
public:
  // algorithm for epipolar model
  enum EpipolarAlgo {
    eCOPLANAR,  // frame coplanar
    ePLANERECT  // rectification upon ground plane
  };

  // position of component images in the stereo model
  enum StereoPos {
    eIGNORE = -1,
    eLEFT = 0,      // left image of stereo pair
    eFORE = eLEFT,  // fore image of triplet
    eNADIR = 1,     // nadir image of triplet
    eRIGHT = 2,     // right image of stereo pair
    eAFT = eRIGHT   // aft image of triplet
  };

  virtual ~IEpipolarModel() = default;

  /**
   *  Epipolar-to-raw transform.
   *
   *  \brief Given a 2d poistin on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in] raw 2d position (x, y) in image raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool EpiToRaw(const Point2d &epi, Point2d &raw) const = 0;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster coordinate
   * system
   *  \param [in] raw List of 2d positions (x, y) in image raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual size_t EpiToRaw(const std::vector<Point2d> &epi, std::vector<Point2d> &raw) const = 0;

  /**
   *  Raw-to-epipolar transform.
   *
   *  \brief Given a 2d poistin on original image, output the corresponding
   *  position on epipolar.
   *
   *  \param [in] raw 2d position (x, y) in image raster coordinate system
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \return Returns true if operation succeeds
   */
  virtual bool RawToEpi(const Point2d &raw, Point2d &epi) const = 0;

  /**
   *  \brief Given a list of 2d poistions on original image, output the 
   *   corresponding positions on epipolar.
   *
   *  \param [in] raw List of 2d positions (x, y) in image raster coordinate
   * system 
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster coordinate system 
   *  \return Returns true if operation succeeds
   */
  virtual size_t RawToEpi(const std::vector<Point2d> &raw, std::vector<Point2d> &epi) const = 0;

  /**
   *  \return Returns algorithm of epipolar model
   */
  virtual EpipolarAlgo GetEpipolarAlgorithm() const noexcept = 0;

  /**
   *  \return Returns raster columns & rows of epipolar image [unit: pixel]
   */
  virtual std::pair<int, int> GetImageSize() const noexcept = 0;

  /**
   *  \return Returns the internal sensor model
   */
  virtual const ISensorModel* GetSensorModel() const noexcept = 0;

  /**
   *  \return Returns raster columns & rows of raw image [unit: pixel]
   */
  virtual std::pair<int, int> GetRawImageSize() const noexcept = 0;
};

using EpiAlgo = IEpipolarModel::EpipolarAlgo;
using EpiStereoPos = IEpipolarModel::StereoPos;
using UPIEpipolarModel = std::unique_ptr<IEpipolarModel>;
using SPIEpipolarModel = std::shared_ptr<IEpipolarModel>;
} // namespace earthxpert
