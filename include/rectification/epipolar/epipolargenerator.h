/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "epipolarmodel/iepipolarmodel.h"
#include "rectification/epipolar/exepipolar.h"
#include <memory>

namespace earthxpert {
class IRasterReader;
class IRasterResample;
class IRasterWriter;
class IEpipolarModel;

/**
 *  The class EpipolarGenerator creates epipolar images from raw images.
 */
class EXEPIPOLAR_EXPORTS EpipolarGenerator {
public:
  /**
   *  \brief
   */
  EpipolarGenerator(const IRasterResample* rasResample,
                    size_t memSize);

  EpipolarGenerator(const EpipolarGenerator &) = delete;
  EpipolarGenerator(EpipolarGenerator &&) = delete;
  EpipolarGenerator &operator=(const EpipolarGenerator &) = delete;
  EpipolarGenerator &operator=(EpipolarGenerator &&) = delete;
  ~EpipolarGenerator() {}

  /**
   *  \brief
   *
   *  \return Returns true if operation succeeds
   */
  bool Process(IRasterReader *const img, IRasterWriter *const epi,
               IEpipolarModel const *const epiModel,
               IEpipolarModel::StereoPos pos);

private:
  const IRasterResample* mRasResample{};
  unsigned short mResampleStride{1}; // stride for rigorous epipolar geometry
  size_t mMemSize{};                 // memory size allowed for processing

  // layout helper
};
} // namespace earthxpert
