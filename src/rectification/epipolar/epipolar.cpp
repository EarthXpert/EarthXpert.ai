/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "core/utils/fsutils.h"
#include "core/utils/rasterresample.h"
#include "rectification/epipolar/epipolar.h"
#include "epipolarmodel/epipolarmodel.h"
#include "epipolarmodel/epipolarmodelfactory.h"
#include "io/raster/rasterreader.h"
#include "io/raster/rasterwriter.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "rectification/epipolar/epipolargenerator.h"

using namespace earthxpert;
using namespace earthxpert::utils;

namespace earthxpert::epipolar {
/**
 *  \brief Generate epipolar images for a pair of stereo image having sensor models.
 */
void GenerateEpipolar(RasterReader* leftReader, const IEpipolarModel* leftEpiModel,
  RasterReader* rightReader, const IEpipolarModel* rightEpiModel,
  RasterWriter* leftEpiWriter, RasterWriter* rightEpiWriter,
  const IRasterResample* rasResample, const size_t memSize) {
  EpipolarGenerator epiGen(rasResample, memSize);

  // generate left epipolar image
  epiGen.Process(leftReader, leftEpiModel, leftEpiWriter);

  // generate right epipolar image
  epiGen.Process(rightReader, rightEpiModel, rightEpiWriter);
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

  // left epipolar image
  auto leftEpiFn = PathPrefixFilename(leftImgFile, "epi_");
  fs::path leftEpiFile(outPath + leftEpiFn);
  if (fs::exists(leftEpiFile)) {
    fs::remove(leftEpiFile);
  }
  auto leftEpiRect = dynamic_cast<GeoEpipolarModel*>(leftEpiModel.get())->GetEpiRect();
  int leftEpiNumCols{ leftEpiRect->GetNumCols() };
  int leftEpiNumRows{ leftEpiRect->GetNumRows() };
  RasterWriter leftEpiWriter(leftEpiFile.string(), format, 
    leftEpiNumCols, leftEpiNumRows, gdalDT, numBands);

  // right epipolar image
  auto rightEpiFn = PathPrefixFilename(rightImgFile, "epi_");
  fs::path rightEpiFile(outPath + rightEpiFn);
  if (fs::exists(rightEpiFile)) {
    fs::remove(rightEpiFile);
  }
  auto rightEpiRect = dynamic_cast<GeoEpipolarModel*>(rightEpiModel.get())->GetEpiRect();
  int rightEpiNumCols{ rightEpiRect->GetNumCols() };
  int rightEpiNumRows{ rightEpiRect->GetNumRows() };
  RasterWriter rightEpiWriter(rightEpiFile.string(), format,
    rightEpiNumCols, rightEpiNumRows, gdalDT, numBands);

  auto rasResample = std::make_unique<RasterResample>(resample);

  GenerateEpipolar(&leftReader, leftEpiModel.get(), &rightReader, rightEpiModel.get(),
    &leftEpiWriter, &rightEpiWriter, rasResample.get(), memSize);
}
}
