/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/kindfillpolicy.h"
#include "core/utils/typehelper.h"
#include "core/exceptions/wxexcept.h"

using namespace earthxpert;

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
ReflectionFill::ReflectionFill(CvRaster* const raster, const int bpp,
  const int readTop, const int readBottom, const int readLeft, const int readRight)
  : mRaster(raster), mBpp(bpp), mReadTop(readTop),
  mReadBottom(readBottom), mReadLeft(readLeft), mReadRight(readRight) {}

/**
 *  \brief fill margin pixel by copying the reflective read pixel
 *
 *  \param [in] c col of marigin pixel
 *  \param [in] r row of marigin pixel
 *  \param [in] cp col of reflective read pixel
 *  \param [in] rp row of reflective read pixel
 */
void ReflectionFill::CopyPixel(const int c, const int r,
  const int cp, const int rp) const {
  auto margin = mRaster->GetPixel(c, r);
  auto read = mRaster->GetPixel(cp, rp);
  memcpy(margin, read, mBpp);
}

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
ReplicationFill::ReplicationFill(CvRaster* const raster, const int bpp,
  const int readTop, const int readBottom, const int readLeft, const int readRight)
  : mRaster(raster), mBpp(bpp), mReadTop(readTop),
  mReadBottom(readBottom), mReadLeft(readLeft), mReadRight(readRight) {}

/**
 *  \brief fill margin pixel by copying the reflective read pixel
 *
 *  \param [in] c col of marigin pixel
 *  \param [in] r row of marigin pixel
 *  \param [in] cp col of reflective read pixel
 *  \param [in] rp row of reflective read pixel
 */
void ReplicationFill::CopyPixel(const int c, const int r,
  const int cp, const int rp) const {
  auto margin = mRaster->GetPixel(c, r);
  auto read = mRaster->GetPixel(cp, rp);
  memcpy(margin, read, mBpp);
}
