/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "rectification/epipolar/epipolar.h"
#include "core/utils/fsutils.h"
#include "core/utils/rasterresample.h"
#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarmodelfactory.h"
#include "io/raster/rasterreader.h"
#include "io/raster/rasterwriter.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "rectification/epipolar/epipolargenerator.h"

using namespace earthxpert;
using namespace earthxpert::utils;

namespace earthxpert {
namespace detail {
/**
 *
 */
void GenerateEpipolar(const std::string& imgFile, EpipolarGenerator* epiGen, 
  IRasterReader* reader, IEpipolarModel* epiModel, const std::string& outPath,
  const std::string& format, const GDALDataType gdalDT, const int numBands) {
  auto epiFn = PathPrefixFilename(imgFile, "epi_");
  fs::path epiFile(outPath + epiFn);
  if (fs::exists(epiFile)) {
    fs::remove(epiFile);
  }

  auto leftEpiRect = dynamic_cast<GeoEpipolarModel*>(epiModel)->GetEpiRect();
  int leftEpiNumCols{ leftEpiRect->GetNumCols() };
  int leftEpiNumRows{ leftEpiRect->GetNumRows() };

  RasterWriter epiWriter(epiFile.string(), format,
    leftEpiNumCols, leftEpiNumRows, gdalDT, numBands);

  epiGen->Process(reader, epiModel, &epiWriter);
}
}

/**
 *  \brief Generate epipolar images for a pair of stereo image having frame models.
 */
void GenerateFrameEpipolar() {
}

/**
 *  \brief Generate epipolar images for a pair of stereo image having rational models.
 */
void GenerateRationalEpipolar(const std::string& leftImgFile, const std::string& leftMdlFile, 
  const std::string& rightImgFile, const std::string& rightMdlFile, 
  const std::vector<int>& bands, const int sample, const std::string& outPath,
  const std::string& resample, const size_t memSize) {
  RationalModelMarshaller rmMarshaller;

  RasterReader leftReader(leftImgFile, bands);
  auto [leftNumCols, leftNumRows] = leftReader.GetSize();
  
  rmMarshaller.SetImageSize(leftNumCols, leftNumRows);
  std::shared_ptr<ISensorModel> leftModel = rmMarshaller.Deserialize(leftMdlFile);
  assert(SenType::eRATIONAL == leftModel->GetModelType());

  RasterReader rightReader(rightImgFile, bands);
  auto [rightNumCols, rightNumRows] = rightReader.GetSize();

  rmMarshaller.SetImageSize(rightNumCols, rightNumRows);
  std::shared_ptr<ISensorModel> rightModel = rmMarshaller.Deserialize(rightMdlFile);
  assert(SenType::eRATIONAL == rightModel->GetModelType());

  EpipolarModelFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = epiModelFac.CreateEpipolarModel(leftModel, rightModel, sample); 
  assert(IEpipolarModel::ePLANERECT == epipolarModel->GetEpipolarAlgorithm());

  auto format = leftReader.GetFileFormat();
  auto gdalDT = leftReader.GetDataType();
  auto numBands = leftReader.GetNumBands();

  auto rasResample = std::make_unique<RasterResample>(resample);
  EpipolarGenerator epiGen(rasResample.get(), memSize);

  // generate left epipolar image
  detail::GenerateEpipolar(leftImgFile, &epiGen, &leftReader, 
    leftEpiModel.get(), outPath, format, gdalDT, numBands);

  // generate right epipolar image
  detail::GenerateEpipolar(rightImgFile, &epiGen, &rightReader,
    rightEpiModel.get(), outPath, format, gdalDT, numBands);
}
}
