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

  auto [epiNumCols, epiNumRows] = epiModel->GetImageSize();
  RasterWriter epiWriter(epiFile.string(), format,
    epiNumCols, epiNumRows, gdalDT, numBands);

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
void GenerateRationalEpipolar(const EpipolarOptions& epiOpt) {
  RationalModelMarshaller rmMarshaller;

  RasterReader leftReader(epiOpt.leftImgFile, epiOpt.bands);
  auto [leftNumCols, leftNumRows] = leftReader.GetSize();
  
  rmMarshaller.SetImageSize(leftNumCols, leftNumRows);
  std::shared_ptr<ISensorModel> leftModel = 
    rmMarshaller.Deserialize(epiOpt.leftMdlFile);
  assert(SensorType::eRATIONAL == leftModel->GetModelType());

  RasterReader rightReader(epiOpt.rightImgFile, epiOpt.bands);
  auto [rightNumCols, rightNumRows] = rightReader.GetSize();

  rmMarshaller.SetImageSize(rightNumCols, rightNumRows);
  std::shared_ptr<ISensorModel> rightModel = 
    rmMarshaller.Deserialize(epiOpt.rightMdlFile);
  assert(SensorType::eRATIONAL == rightModel->GetModelType());

  EpipolarModelFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = 
    epiModelFac.CreateEpipolarModel(leftModel, rightModel, epiOpt.sample);
  assert(IEpipolarModel::ePLANERECT == leftEpiModel->GetEpipolarAlgorithm());

  auto format = leftReader.GetFileFormat();
  auto gdalDT = leftReader.GetDataType();
  auto numBands = leftReader.GetNumBands();

  auto rasResample = std::make_unique<RasterResample>(epiOpt.resample);
  EpipolarGenerator epiGen(rasResample.get(), epiOpt.memSize);

  // generate left epipolar image
  detail::GenerateEpipolar(epiOpt.leftImgFile, &epiGen, &leftReader,
    leftEpiModel.get(), epiOpt.outPath, format, gdalDT, numBands);

  // generate right epipolar image
  detail::GenerateEpipolar(epiOpt.rightImgFile, &epiGen, &rightReader,
    rightEpiModel.get(), epiOpt.outPath, format, gdalDT, numBands);
}
}
