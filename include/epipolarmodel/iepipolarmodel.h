/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rd_party/opencv/wxopencv.h"
#include "epipolarmodel/wxepipolarmodel.h"

#include <vector>

namespace watxtract {
/**
 *  The interface IEpipolarModel defines the geometric transform
 *  between the epipolar image and the associated image product
 *  used to generate it.
 */
class WXEPIPOLARMODEL_EXPORTS IEpipolarModel {
public:
  // algorithm for epipolar model
  enum EpipolarAlgo {
    eCOPLANAR,  // frame coplanar
    ePLANERECT  // rectification upon ground plane
  };

  // position of component images in the stereo model
  enum StereoPos {
    eIGNORE,
    eLEFT, // left image of stereo pair
    eRIGHT // right image of stereo pair
  };

  virtual ~IEpipolarModel() {}

  /**
   *  Epipolar-to-image transform.
   *
   *  \brief Given a 2d poistin on epipolar, output the corresponding position
   * on original image.
   *
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in] img 2d position (x, y) in image raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  virtual bool EpipolarToImage(const Point2d &epi, Point2d &img,
                               StereoPos pos) const = 0;

  /**
   *  \brief Given a list of 2d poistions on epipolar, output the corresponding
   * positions on original image.
   *
   *  \param [in] epi List of 2d positions (x, y) in epipolar raster coordinate
   * system 
   *  \param [in] img List of 2d positions (x, y) in image raster
   * coordinate system 
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  virtual size_t EpipolarToImage(const std::vector<Point2d> &epi,
                                 std::vector<Point2d> &img,
                                 StereoPos pos) const = 0;

  /**
   *  Image-to-epipolar transform.
   *
   *  \brief Given a 2d poistin on image, output the corresponding position on
   * epipolar.
   *
   *  \param [in] img 2d position (x, y) in image raster coordinate system
   *  \param [in] epi 2d position (x, y) in epipolar raster coordinate system
   *  \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  virtual bool ImageToEpipolar(const Point2d &img, Point2d &epi,
                               StereoPos pos) const = 0;

  /**
   *  \brief Given a list of 2d poistions on image, output the corresponding
   * positions on epipolar.
   *
   *  \param [in] img List of 2d positions (x, y) in image raster coordinate
   * system \param [in] epi List of 2d positions (x, y) in epipolar raster
   * coordinate system \param [in] pos Component position of stereo model
   *  \return Returns true if operation succeeds
   */
  virtual size_t ImageToEpipolar(const std::vector<Point2d> &img,
                                 std::vector<Point2d> &epi,
                                 StereoPos pos) const = 0;

  /**
   *  \return Returns algorithm of epipolar model
   */
  virtual EpipolarAlgo GetEpipolarAlgorithm() const noexcept = 0;
};
} // namespace watxtract