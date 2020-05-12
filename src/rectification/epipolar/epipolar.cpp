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

using namespace watxtract;
using namespace watxtract::utils;
using FacEpiOpt = EpipolarModelFactory::EpiOpt;
using SenType = ISensorModel::ModelType;

namespace watxtract::epipolar {
/**
 *  \brief Generate epipolar images for a pair of stereo image having sensor models.
 */
void GenerateEpipolar(RasterReader* leftReader, const ISensorModel* leftModel,
  RasterReader* rightReader, const ISensorModel* rightModel,
  const IEpipolarModel* epipolarModel,
  RasterWriter* leftEpiWriter, RasterWriter* rightEpiWriter,
  const IRasterResample* rasResample, const size_t memSize) {
  auto epiModel = dynamic_cast<const GeoEpipolarModel*>(epipolarModel);
  EpipolarGenerator epiGen(rasResample, memSize);

  // generate left epipolar image
  epiGen.Process(leftReader, leftEpiWriter, epiModel, IEpipolarModel::eLEFT);

  // generate right epipolar image
  epiGen.Process(rightReader, rightEpiWriter, epiModel, IEpipolarModel::eRIGHT);
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
  auto epipolarModel = epiModelFac.CreateEpipolarModel(leftModel, rightModel, sample); 
  assert(IEpipolarModel::ePLANERECT == epipolarModel->GetEpipolarAlgorithm());

  auto epiModel = dynamic_cast<GeoEpipolarModel*>(epipolarModel.get());

  auto format = leftReader.GetFileFormat();
  auto gdalDT = leftReader.GetDataType();
  auto numBands = leftReader.GetNumBands();

  // left epipolar image
  auto leftEpiFn = PathPrefixFilename(leftImgFile, "epi_");
  fs::path leftEpiFile(outPath + leftEpiFn);
  if (fs::exists(leftEpiFile)) {
    fs::remove(leftEpiFile);
  }
  auto leftEpiRect = epiModel->GetEpiRect(IEpipolarModel::eLEFT);
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
  auto rightEpiRect = epiModel->GetEpiRect(IEpipolarModel::eRIGHT);
  int rightEpiNumCols{ rightEpiRect->GetNumCols() };
  int rightEpiNumRows{ rightEpiRect->GetNumRows() };
  RasterWriter rightEpiWriter(rightEpiFile.string(), format,
    rightEpiNumCols, rightEpiNumRows, gdalDT, numBands);

  auto rasResample = std::make_unique<RasterResample>(resample);

  GenerateEpipolar(&leftReader, leftModel.get(), &rightReader, rightModel.get(),
    epipolarModel.get(), &leftEpiWriter, &rightEpiWriter, rasResample.get(), memSize);
}
}
