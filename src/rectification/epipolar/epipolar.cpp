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
#include "marshaller/epipolarmodelmarshaller.h"
#include "marshaller/rationalmodelmarshaller.h"
#include "rectification/epipolar/epipolargenerator.h"

#include <boost/format.hpp>
#include <iostream>
#include <sstream>

using namespace earthxpert;
using namespace earthxpert::utils;

namespace earthxpert {
namespace detail {
/**
 *  \return Return file path of the generated epipolar image
 */
std::string GenerateEpipolar(const std::string& imgFile, EpipolarGenerator* epiGen, 
  IRasterReader* reader, IEpipolarModel* epiModel, const int stride, 
  const int outBkgd, const std::string& outPath, const std::string& format, 
  const GDALDataType gdalDT, const int numBands) {
  //std::cout << "rawFile: " << imgFile << std::endl;
  //const auto [rawNumCols, rawNumRows] = reader->GetSize();
  //std::cout << "rawNumCols: " << rawNumCols << ", rawNumRows: " << rawNumRows << std::endl;

  std::stringstream ss;
  ss << boost::format("epi%1%_") % stride;
  auto epiFn = PathPrefixFilename(imgFile, ss.str());
  fs::path epiPath(outPath + epiFn);
  RemoveFile(epiPath);
  std::cout << "epiPath: " << epiPath << std::endl;

  auto [epiNumCols, epiNumRows] = epiModel->GetImageSize();
  RasterWriter epiWriter(epiPath, format,
    epiNumCols, epiNumRows, gdalDT, numBands);
  //std::cout << "epiNumCols: " << epiNumCols << ", epiNumRows: " << epiNumRows << std::endl;

  epiGen->Process(reader, &epiWriter, epiModel);
  epiWriter.SetNodataValues(outBkgd);
  return epiPath.string();
}
}

std::string ToString(const EpipolarOptions* epiOpt) {
  std::stringstream ss;
  ss << boost::format("** EpipolarOptions **\n"
    "sensorName: %1%\nleftImgFile: %2%\nleftMdlFile: %3%\n"
    "rightImgFile: %4%\nrightMdlFile: %5%\nbands: %6%\nsample: %7%\n"
    "stride: %8%\ngrey: %9%\noutPath: %10%\nresample: %11%\nmemSize: %12%\n") % \
    epiOpt->sensorName % epiOpt->leftImgFile % epiOpt->leftMdlFile % \
    epiOpt->rightImgFile % epiOpt->rightMdlFile % epiOpt->bands.size() %\
    epiOpt->sample % epiOpt->stride % epiOpt->grey % epiOpt->outPath % \
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

  EpipolarModelFactory epiModelFac(epiOpt.outOpt);
  auto [leftEpiModel, rightEpiModel] = 
    epiModelFac.CreateRationalEpiModel(leftModel, rightModel, epiOpt.sample);
  assert(IEpipolarModel::ePLANERECT == leftEpiModel->GetEpipolarAlgorithm());

  auto rasResample = std::make_unique<RasterResampleCV>(epiOpt.resample);
  EpipolarGenerator epiGen(rasResample.get(), epiOpt.memSize, epiOpt.stride);

  // generate left epipolar image
  auto format = leftReader.GetFileFormat();
  auto gdalDT = leftReader.GetDataType();
  auto numBands = leftReader.GetNumBands();
  int epiNumBands = (epiOpt.grey && 3 == numBands) ? 1 : numBands;

  auto leftEpiFile = detail::GenerateEpipolar(epiOpt.leftImgFile, 
    &epiGen, &leftReader, leftEpiModel.get(), epiOpt.stride,
    epiOpt.outBkgd, epiOpt.outPath, format, gdalDT, epiNumBands);
  std::cout << ToString(epiGen.GetPerfMettrics());

  // generate right epipolar image, allowing for in-compatible image files
  format = rightReader.GetFileFormat();   
  gdalDT = rightReader.GetDataType();
  numBands = rightReader.GetNumBands();
  epiNumBands = (epiOpt.grey && 3 == numBands) ? 1 : numBands;

  auto rightEpiFile = detail::GenerateEpipolar(epiOpt.rightImgFile,
    &epiGen, &rightReader, rightEpiModel.get(), epiOpt.stride, 
    epiOpt.outBkgd, epiOpt.outPath, format, gdalDT, epiNumBands);
  std::cout << ToString(epiGen.GetPerfMettrics());

  // save epipolar model information
  std::string leftImgFile = fs::path(epiOpt.leftImgFile).stem().string();
  std::string rightImgFile = fs::path(epiOpt.rightImgFile).stem().string();
  const auto epiModelFn = CombineFilenames(leftImgFile, rightImgFile);
  fs::path epiModelPath(epiOpt.outPath + epiModelFn + ".json");
  RemoveFile(epiModelPath);
  std::cout << "epiModelPath: " << epiModelPath << std::endl << std::endl;

  std::vector<EpiModelInfo> epiModels{ 
    {epiOpt.leftMdlFile, leftEpiFile, leftEpiModel.get()},
    {epiOpt.rightMdlFile, rightEpiFile, rightEpiModel.get()} };

  EpipolarStereoMarshaller marshaller;
  marshaller.Serialize(epiModelPath.string(), epiModels);
}
}
