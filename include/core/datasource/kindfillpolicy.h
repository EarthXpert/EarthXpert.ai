/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/datasource/ikindfillpolicy.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/rastertypes.h"

#include <cstring>

namespace earthxpert {
/**
 *  \brief Set a constant value, e.g. 0, to the margin pixels.
 *
 *  \todo accept a constant value.
 */
class EXCORE_EXPORTS ConstantFill final : public IKindFillPolicy {
public:
  /**
   *  Constructor
   *
   *  \param [in/out] raster the raster object whose margins to be filled
   *  \param [in] bpp bytes per pixel
   */
  ConstantFill(CvRaster* const raster, const int bpp, int, int, int, int)
    : mRaster(raster), mBpp(bpp) {}
  ConstantFill(const ConstantFill&) = delete;
  ConstantFill(ConstantFill&&) = delete;
  ~ConstantFill() = default;

  ConstantFill& operator=(const ConstantFill& rhs) = delete;
  ConstantFill& operator=(ConstantFill&& rhs) noexcept = delete;

  /**
   *  \brief fill margin pixel on top side
   *
   *  \param [in] c col of margin pixel
   *  \param [in] r row of margin pixel
   */
  void FillTopPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on bottom side
   */
  void FillBottomPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on left side
   */
  void FillLeftPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on right side
   */
  void FillRightPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on top-left corner
   */
  void FillTLPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on top-right corner
   */
  void FillTRPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on bottom-left corner
   */
  void FillBLPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

  /**
   *  \brief fill margin pixel on bottom-right corner
   */
  void FillBRPixel(const int c, const int r) override {
    FillPixel(c, r);
  }

private:
  CvRaster* mRaster{ nullptr };   // raster object to fill margins
  int mBpp{};   // number of bytes per pixel

  inline void FillPixel(const int c, const int r) {
    auto pixel = mRaster->GetPixel(c, r);
    memset(pixel, 0, mBpp);
  }
};

/**
 *  \brief Set the opposite read pixel to the margin pixel, reflecting w.r.t. the read border.
 */
class EXCORE_EXPORTS ReflectionFill final : public IKindFillPolicy {
public:
  /**
   *  Constructor
   *
   *  \param [in/out] raster the raster object whose margins to be filled
   *  \param [in] bpp bytes per pixel
   *  \param [in] readTop start row read from image file
   *  \param [in] readBottom one row below the last row read from image file
   *  \param [in] readLeft start col read from image file
   *  \param [in] readRight one col on the right of the last col read from image file
   */
  ReflectionFill(CvRaster* const raster, int bpp,
    int readTop, int readBottom, int readLeft, int readRight);
  ReflectionFill(const ReflectionFill&) = delete;
  ReflectionFill(ReflectionFill&&) = delete;
  ~ReflectionFill() = default;

  ReflectionFill& operator=(const ReflectionFill& rhs) = delete;
  ReflectionFill& operator=(ReflectionFill&& rhs) noexcept = delete;

  /**
   *  \brief fill margin pixel on top side
   *
   *  \param [in] c col of margin pixel
   *  \param [in] r row of margin pixel
   */
  void FillTopPixel(int c, int r) override {
    const int rp = mReadTop + mReadTop - r - 1;
    CopyPixel(c, r, c, rp);
  }

  /**
   *  \brief fill margin pixel on bottom side
   */
  void FillBottomPixel(int c, int r) override {
    const int rp = mReadBottom - (r - mReadBottom) - 1;
    CopyPixel(c, r, c, rp);
  }

  /**
   *  \brief fill margin pixel on left side
   */
  void FillLeftPixel(int c, int r) override {
    const int cp = mReadLeft + (mReadLeft - c) - 1;
    CopyPixel(c, r, cp, r);
  }

  /**
   *  \brief fill margin pixel on right side
   */
  void FillRightPixel(int c, int r) override {
    const int cp = mReadRight - (c - mReadRight) - 1;
    CopyPixel(c, r, cp, r);
  }

  /**
   *  \brief fill margin pixel on top-left corner
   */
  void FillTLPixel(int c, int r) override {
    const int cp = mReadLeft + (mReadLeft - c) - 1;
    const int rp = mReadTop + mReadTop - r - 1;
    CopyPixel(c, r, cp, rp);
  }

  /**
   *  \brief fill margin pixel on top-right corner
   */
  void FillTRPixel(int c, int r) override {
    const int cp = mReadRight - (c - mReadRight) - 1;
    const int rp = mReadTop + mReadTop - r - 1;
    CopyPixel(c, r, cp, rp);
  }

  /**
   *  \brief fill margin pixel on bottom-left corner
   */
  void FillBLPixel(int c, int r) override {
    const int cp = mReadLeft + (mReadLeft - c) - 1;
    const int rp = mReadBottom - (r - mReadBottom) - 1;
    CopyPixel(c, r, cp, rp);
  }

  /**
   *  \brief fill margin pixel on bottom-right corner
   */
  void FillBRPixel(int c, int r) override {
    const int cp = mReadRight - (c - mReadRight) - 1;
    const int rp = mReadBottom - (r - mReadBottom) - 1;
    CopyPixel(c, r, cp, rp);
  }

private:
  CvRaster* mRaster{ nullptr };   // raster object to fill margins
  int mBpp{};   // number of bytes per pixel
  int mReadTop{};
  int mReadBottom{};
  int mReadLeft{};
  int mReadRight{};

  void CopyPixel(int c, int r, int cp, int rp) const;
};

/**
 *  \brief Duplicate the nearest border pixel to the margin pixel.
 */
class EXCORE_EXPORTS ReplicationFill final : public IKindFillPolicy {
public:
  /**
   *  Constructor
   *
   *  \param [in/out] raster the raster object whose margins to be filled
   *  \param [in] bpp bytes per pixel
   *  \param [in] readTop start row read from image file
   *  \param [in] readBottom one row below the last row read from image file
   *  \param [in] readLeft start col read from image file
   *  \param [in] readRight one col on the right of the last col read from image file
   */
  ReplicationFill(CvRaster* const raster, int bpp,
    int readTop, int readBottom, int readLeft, int readRight);

  /**
   *  \brief fill margin pixel on top side
   *
   *  \param [in] c col of margin pixel
   *  \param [in] r row of margin pixel
   */
  void FillTopPixel(int c, int r) override {
    CopyPixel(c, r, c, mReadTop);
  }

  /**
   *  \brief fill margin pixel on bottom side
   */
  void FillBottomPixel(int c, int r) override {
    CopyPixel(c, r, c, mReadBottom-1);
  }

  /**
   *  \brief fill margin pixel on left side
   */
  void FillLeftPixel(int c, int r) override {
    CopyPixel(c, r, mReadLeft, r);
  }

  /**
   *  \brief fill margin pixel on right side
   */
  void FillRightPixel(int c, int r) override {
    CopyPixel(c, r, mReadRight-1, r);
  }

  /**
   *  \brief fill margin pixel on top-left corner
   */
  void FillTLPixel(int c, int r) override {
    CopyPixel(c, r, mReadLeft, mReadTop);
  }

  /**
   *  \brief fill margin pixel on top-right corner
   */
  void FillTRPixel(int c, int r) override {
    CopyPixel(c, r, mReadRight-1, mReadTop);
  }

  /**
   *  \brief fill margin pixel on bottom-left corner
   */
  void FillBLPixel(int c, int r) override {
    CopyPixel(c, r, mReadLeft, mReadBottom-1);
  }

  /**
   *  \brief fill margin pixel on bottom-right corner
   */
  void FillBRPixel(int c, int r) override {
    CopyPixel(c, r, mReadRight-1, mReadBottom - 1);
  }

private:
  CvRaster* mRaster{ nullptr };   // raster object to fill margins
  int mBpp{};   // number of bytes per pixel
  int mReadTop{};
  int mReadBottom{};
  int mReadLeft{};
  int mReadRight{};

  void CopyPixel(int c, int r, int cp, int rp) const;
};
}
