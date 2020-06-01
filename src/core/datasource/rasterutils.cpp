/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/datasource/utils.h"
#include "core/datasource/kindfillpolicy.h"
#include "core/datasource/marginfill.h"
#include "core/exceptions/exexcept.h"

#include <map>
#include <numeric>

namespace earthxpert {
// local details
namespace detail {
  // Pixel data types of GDAL & CV
  static const std::map<GDALDataType, int> sDataTypesG2C = {
    {GDT_Byte, CV_8U},
    //{GDT_Int8, CV_8S},
    {GDT_UInt16, CV_16U},
    {GDT_Int16, CV_16S},
    //{GDT_UInt32, CV_32U},
    {GDT_Int32, CV_32S},
    {GDT_Float32, CV_32F},
    {GDT_Float64, CV_64F} };

  static const std::map<int, GDALDataType> sDataTypesC2G = {
    {CV_8U, GDT_Byte},
    //{CV_8S, GDT_Byte},
    {CV_16U, GDT_UInt16},
    {CV_16S, GDT_Int16},
    //{CV_32U, GDT_UInt32},
    {CV_32S, GDT_Int32},
    {CV_32F, GDT_Float32},
    {CV_64F, GDT_Float64} };

  /**
   *  \brief Fill margins for given sides using the templated policy kind.
   *
   *  \param KindFillPolicy template parameter specifies the filling policy kind.
   *  \param [in/out] raster the raster object whose margins will be filled.
   *  \param [in] dim tells the dimensions of the four sides.
   */
  template<typename KindFillPolicy>
  void FillMargin(CvRaster* const raster, const MarginFillDim dim) {
    MarginFill<KindFillPolicy> marginFill(raster, dim);

    marginFill.Run();
  }
}

/**
 *  \brief Convert pixel type from GDAL to OpenCV
 *
 *  \param [in] gdalType
 *  \param [in] numBands
 *  \return Returns CV type
 */
int ConvertPixelType(const GDALDataType gdalType, const int numBands) {
  const auto it = detail::sDataTypesG2C.find(gdalType);
  if (detail::sDataTypesG2C.end() == it) {
    throw std::invalid_argument("Unknown GDAL data type");
  }

  return CV_MAKETYPE(it->second, numBands);
}

/**
 *  \brief Convert pixel type from OpenCV to GDAL
 *
 *  \param [in] cvType
 *  \return Returns GDAL data type and band numbers
 */
std::pair<GDALDataType, int> EXCORE_EXPORTS ConvertPixelType(const int cvType) {
  const auto depth = CV_MAT_DEPTH(cvType);

  const auto it = detail::sDataTypesC2G.find(depth);
  if (detail::sDataTypesC2G.end() == it) {
    throw std::invalid_argument("Unknown OpenCV depth type");
  }

  return {it->second, CV_MAT_CN(cvType) };
}

/**
 *  \brief fill target pixel by copying the source pixel
 *
 *  \param [in] raster pointer to raster object
 *  \param [in] c col of target pixel
 *  \param [in] r row of target pixel
 *  \param [in] cp col of source pixel
 *  \param [in] rp row of source pixel
 */
void CopyPixel(const CvRaster* raster, const int c, const int r, const int cp, const int rp) {
  auto tgt = raster->GetPixel(c, r);
  auto src = raster->GetPixel(cp, rp);
  const int bpp{raster->GetBytesPerPixel()};
  memcpy(tgt, src, bpp);
}

/**
 *  \brief set target pixel
 *
 *  \param [in] raster pointer to raster object
 *  \param [in] c col of target pixel
 *  \param [in] r row of target pixel
 *  \param [in] src source pixel
 */
void SetPixel(const IRaster* raster, const int c, const int r, const uchar* src) {
  auto tgt = raster->GetPixel(c, r);
  const int bpp{ raster->GetBytesPerPixel() };
  memcpy(tgt, src, bpp);
}

/**
 *  \brief Fill margins, if needed, for OpenCV raster object.
 *
 *  \param [in/out] raster the raster object already populated with the pixel block,
 *                  and ready for filling margins.
 *  \param [in] dim tells the dimensions of the four sides.
 */
void FillMargins(CvRaster* const raster, MarginFillDim dim) {
  switch (raster->GetMarginKind()) {
  case MarginKind::eCONSTANT:
    detail::FillMargin<ConstantFill>(raster, dim);
    break;
  case MarginKind::eREFLECTION:
    detail::FillMargin<ReflectionFill>(raster, dim);
    break;
  case MarginKind::eREPLICATION:
    detail::FillMargin<ReplicationFill>(raster, dim);
    break;
  default:
    throw std::invalid_argument("Invalid margin kind");
  }
}

/**
 *  \brief Convert raster object from GDAL array to OpenCV Mat.
 *  If only one band is needed, consider to use the overloadded version.
 *
 *  \param [in] gdalRaster pointer to GDAL raster object
 *  \return Return pointer to OpenCV raster object
 */
std::unique_ptr<CvRaster> ConvertRaster(const GdalRaster* gdalRaster) {
  auto [numCols, numRows] = gdalRaster->GetSize();
  auto gdalType = gdalRaster->GetDataType();
  auto numBands = gdalRaster->GetNumBands();
  auto [colOffset, rowOffset] = gdalRaster->GetOffsets();
  auto [marginCols, marginRows] = gdalRaster->GetMarginSize();
  auto marginKind = gdalRaster->GetMarginKind();
  auto gdalBuffer = gdalRaster->GetBuffer();
  auto bufNumBytes = gdalRaster->GetBufferNumBytes();
  auto bpb = gdalRaster->GetBytesPerBand();
  auto [bufNumCols, bufNumRows] = gdalRaster->GetBufferSize();
  auto bandNumBytes = gdalRaster->GetBandNumBytes();

  auto cvType = ConvertPixelType(gdalType, numBands);
  auto cvRaster = std::make_unique<CvRaster>(numCols, numRows, cvType,
    colOffset, rowOffset, marginCols, marginRows, marginKind);
  auto cvBuffer = cvRaster->GetBuffer();

  const int64 numPixels = int64(bufNumCols) * int64(bufNumRows);

  // convert RGB of GDAL into BGR of CVMAT
  auto pixel{ cvBuffer };
  auto bandPage{gdalBuffer + bufNumBytes - bandNumBytes };
  for (int64 p = 0; p < numPixels; ++p) {
    auto band{ bandPage };
    for (int b = numBands - 1; b >= 0; --b) {
      memcpy(pixel, band, bpb);
      pixel += bpb;
      band -= bandNumBytes;
    }

    bandPage += bpb;
  }

  return cvRaster;
}

/**
 *  \brief Give an external interface for the internal GDAL buffer for a single band.
 *  It is particulaly useful to avoid the extra copy when the bands can be manipulated
 *  independently, e.g., for resampling.
 *
 *  \param [in] gdalRaster pointer to GDAL raster object
 *  \param [in] band 0-based band index
 *  \return Returns a cv::Mat that points to the internal buffer
 *  but has no ownership.
 */
std::unique_ptr<CvRaster> ConvertRaster(const GdalRaster* gdalRaster, const int band) {
  auto numBands = gdalRaster->GetNumBands();
  if (band >= numBands) {
    throw std::out_of_range("band ID is out of range");
  }

  auto [numCols, numRows] = gdalRaster->GetSize();
  auto gdalType = gdalRaster->GetDataType();
  auto [colOffset, rowOffset] = gdalRaster->GetOffsets();
  auto [marginCols, marginRows] = gdalRaster->GetMarginSize();
  auto marginKind = gdalRaster->GetMarginKind();
  auto buffer = gdalRaster->GetBuffer() + gdalRaster->GetBandNumBytes() * band;

  auto cvType = ConvertPixelType(gdalType, 1);
  return std::make_unique<CvRaster>(numCols, numRows, cvType,
    colOffset, rowOffset, marginCols, marginRows, marginKind, buffer);
}

/**
 *  \brief Give an external interface for the internal GDAL buffer for specified bands.
 *  It is particulaly useful to avoid the extra copy when the bands can be manipulated
 *  independently, e.g., for resampling.
 *
 *  \param [in] gdalRaster pointer to GDAL raster object
 *  \param [in] bands 0-based indices, if empty, default to all bands.
 *              The order of CV bands follows the indices specified by this parameter.
 *  \return Returns a vector of cv::Mat that points to the internal buffer
 *  but has no ownership.
 */
std::vector<std::unique_ptr<CvRaster>> EXCORE_EXPORTS ConvertRaster(
  const GdalRaster* gdalRaster, const std::vector<int>& bands) {
  std::vector<int> cvtBands{bands};
  const int numBands = gdalRaster->GetNumBands();
  if (numBands < bands.size()) {
    throw std::invalid_argument("Requested bands are larger than the available bands");
  }
  if (bands.empty()) {
    cvtBands.resize(numBands);
    std::iota(cvtBands.begin(), cvtBands.end(), 0);
  }
  else {
    auto [itMin, itMax] = std::minmax_element(cvtBands.begin(), cvtBands.end());
    if (0 > * itMin || numBands <= *itMax) {
      throw std::out_of_range("Requested bands are out of the range of available bands");
    }
  }

  std::vector<std::unique_ptr<CvRaster>> cvRasters;
  for (auto b : cvtBands) {
    cvRasters.push_back(ConvertRaster(gdalRaster, b));
  }

  return cvRasters;
}

/**
 *  \brief Convert raster object from Opencv Mat to GDAL array
 *
 *  \param [in] cvRaster pointer to OpenCV raster object
 *  \return Return pointer to GDAL raster object
 */
std::unique_ptr<GdalRaster> ConvertRaster(const CvRaster* gdalRaster) {
  return nullptr;
}
}