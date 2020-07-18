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

#include "boost/format.hpp"
#include <iostream>
#include <sstream>

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
  std::cout << "epiFile: " << epiFile << std::endl;

  auto [epiNumCols, epiNumRows] = epiModel->GetImageSize();
  RasterWriter epiWriter(epiFile.string(), format,
    epiNumCols, epiNumRows, gdalDT, numBands);

  epiGen->Process(reader, &epiWriter, epiModel);
}
}

std::string ToString(const EpipolarOptions* epiOpt) {
  std::stringstream ss;
  ss << boost::format("** EpipolarOptions **\n"
    "sensorName: %1%\nleftImgFile: %2%\nleftMdlFile: %3%\n"
    "rightImgFile: %4%\nrightMdlFile: %5%\nbands: %6%\nsample: %7%\n"
    "grey: %8%\noutPath: %9%\nresample: %10%\nmemSize: %11%\n") % \
    epiOpt->sensorName % epiOpt->leftImgFile % epiOpt->leftMdlFile % \
    epiOpt->rightImgFile % epiOpt->rightMdlFile % epiOpt->bands.size() %\
    epiOpt->sample % epiOpt->grey % epiOpt->outPath % \
    epiOpt->resample % epiOpt->memSize;

  return ss.str();
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
  std::shared_ptr<RationalModel> leftModel = 
    rmMarshaller.Deserialize(epiOpt.leftMdlFile);
  assert(SensorType::eRATIONAL == leftModel->GetModelType());

  RasterReader rightReader(epiOpt.rightImgFile, epiOpt.bands);
  auto [rightNumCols, rightNumRows] = rightReader.GetSize();

  rmMarshaller.SetImageSize(rightNumCols, rightNumRows);
  std::shared_ptr<RationalModel> rightModel =
    rmMarshaller.Deserialize(epiOpt.rightMdlFile);
  assert(SensorType::eRATIONAL == rightModel->GetModelType());

  EpipolarModelFactory epiModelFac(FacEpiOpt::eFULL);
  auto [leftEpiModel, rightEpiModel] = 
    epiModelFac.CreateRationalEpiModel(leftModel, rightModel, epiOpt.sample);
  assert(IEpipolarModel::ePLANERECT == leftEpiModel->GetEpipolarAlgorithm());

  auto format = leftReader.GetFileFormat();
  auto gdalDT = leftReader.GetDataType();
  auto numBands = leftReader.GetNumBands();

  const int epiNumBands = (epiOpt.grey && 3 == numBands) ? 1 : numBands;

  auto rasResample = std::make_unique<RasterResampleCV>(epiOpt.resample);
  EpipolarGenerator epiGen(rasResample.get(), epiOpt.memSize);

  // generate left epipolar image
  detail::GenerateEpipolar(epiOpt.leftImgFile, &epiGen, &leftReader,
    leftEpiModel.get(), epiOpt.outPath, format, gdalDT, epiNumBands);
  std::cout << ToString(epiGen.GetPerfMettrics());

  // generate right epipolar image
  detail::GenerateEpipolar(epiOpt.rightImgFile, &epiGen, &rightReader,
    rightEpiModel.get(), epiOpt.outPath, format, gdalDT, epiNumBands);
  std::cout << ToString(epiGen.GetPerfMettrics());
}
}
