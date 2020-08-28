/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"

namespace earthxpert {
class ISensorModel;

/**
 *  The class NullEpipolarModel provide mock functios doing nothing.
 */
class EXEPIPOLARMODEL_EXPORTS NullEpipolarModel final : public IEpipolarModel {
public:
  ~NullEpipolarModel() = default;

  bool EpiToRaw(const Point2d& epi, Point2d& raw) const override { return true; }

  size_t EpiToRaw(const std::vector<Point2d> &epi, 
    std::vector<Point2d> &raw) const override { return 0; }

  bool RawToEpi(const Point2d& raw, Point2d& epi) const override { return true; }

  size_t RawToEpi(const std::vector<Point2d> &raw, 
    std::vector<Point2d> &epi) const override { return 0; }

  EpipolarAlgo GetEpipolarAlgorithm() const noexcept override {
    return EpipolarAlgo::eUNKNOWN; 
  }

  StereoPos GetStereoPos() const noexcept override { return StereoPos::eIGNORE; }

  std::pair<int, int> GetImageSize() const noexcept override { return {0, 0}; }

  const ISensorModel* GetSensorModel() const noexcept override { return nullptr; }

  const Polygon2d& GetBoundary() const noexcept override { return mBound; }

  std::pair<int, int> GetRawImageSize() const noexcept override { return {0, 0}; }

private:
  Polygon2d mBound;
};

using UPNullEpipolarModel = std::unique_ptr<NullEpipolarModel>;
using SPNullEpipolarModel = std::shared_ptr<NullEpipolarModel>;
} // namespace earthxpert
