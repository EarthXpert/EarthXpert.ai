/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/cvraster.h"
#include "core/datasource/rastertypes.h"

namespace earthxpert {
/**
 *  \brief Defines filling behaviour according to both policy kind and the margin sieds.
 *
 *  \param KindFillPolicy template parameter specifies the filling policy kind.
 */
template <class KindFillPolicy>
class MarginFill {
public:
  using PolicyType = KindFillPolicy;

  /**
   *  Constructor
   *
   *  \param [in/out] raster the raster object whose margins to be filled
   *  \param [in] dim tells the dimensions of the four sides.
   */
  MarginFill(CvRaster* const raster, const MarginFillDim dim) : mRaster(raster), mDim(dim) {
    mBpp = mRaster->GetBytesPerPixel();
    std::tie(mNumCols, mNumRows) = mRaster->GetSize();
    std::tie(mMarginCols, mMarginRows) = mRaster->GetMarginSize();
    mColMax = mNumCols + mMarginCols;
    mRowMax = mNumRows + mMarginRows;
    mReadTop = mDim[0] - mMarginRows;
    mReadBottom = mRowMax - mDim[1];
    mReadLeft = mDim[2] - mMarginCols;
    mReadRight = mColMax - mDim[3];

    mKindFill = std::make_unique<KindFillPolicy>(mRaster, mBpp,
      mReadTop, mReadBottom, mReadLeft, mReadRight);
  }

  /**
   *  \brief fill all margins
   */
  void Run() {
    FillTop();
    FillBottom();
    FillLeft();
    FillRight();

    FillTL();
    FillTR();
    FillBL();
    FillBR();
  }

  /**
   *  \brief fill the top side margin
   */
  void FillTop() {
    for (int r = -mMarginRows; r < mReadTop; ++r) {
      for (int c = mReadLeft; c < mReadRight; ++c) {
        mKindFill->FillTopPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the bottom side margin
   */
  void FillBottom() {
    for (int r = mReadBottom; r < mRowMax; ++r) {
      for (int c = mReadLeft; c < mReadRight; ++c) {
        mKindFill->FillBottomPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the left side margin
   */
  void FillLeft() {
    for (int r = mReadTop; r < mReadBottom; ++r) {
      for (int c = -mMarginCols; c < mReadLeft; ++c) {
        mKindFill->FillLeftPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the right side margin
   */
  void FillRight() {
    for (int r = mReadTop; r < mReadBottom; ++r) {
      for (int c = mReadRight; c < mColMax; ++c) {
        mKindFill->FillRightPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the top-left corner margin
   */
  void FillTL()
  {
    for (int r = -mMarginRows; r < mReadTop; ++r) {
      for (int c = -mMarginCols; c < mReadLeft; ++c) {
        mKindFill->FillTLPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the top-right corner margin
   */
  void FillTR()
  {
    for (int r = -mMarginRows; r < mReadTop; ++r) {
      for (int c = mReadRight; c < mColMax; ++c) {
        mKindFill->FillTRPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the bottom-left side margin
   */
  void FillBL() {
    for (int r = mReadBottom; r < mRowMax; ++r) {
      for (int c = -mMarginCols; c < mReadLeft; ++c) {
        mKindFill->FillBLPixel(c, r);
      }
    }
  }

  /**
   *  \brief fill the bottom-right side margin
   */
  void FillBR() {
    for (int r = mReadBottom; r < mRowMax; ++r) {
      for (int c = mReadRight; c < mColMax; ++c) {
        mKindFill->FillBRPixel(c, r);
      }
    }
  }

private:
  CvRaster* mRaster{ nullptr };   // raster object to fill margins
  MarginFillDim mDim{};   // filling dimensions

  std::unique_ptr<KindFillPolicy> mKindFill;  // filler object

  // cached quantities
  int mBpp{};
  int mNumCols{};
  int mNumRows{};
  int mMarginCols{};
  int mMarginRows{};
  int mColMax{};
  int mRowMax{};
  int mReadTop{};
  int mReadBottom{};
  int mReadLeft{};
  int mReadRight{};
};
}
