/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "testconfig.h"

#include "core/utils/rasterresample.h"

using namespace earthxpert;


const float FLT_BASESIGMA = 0.5f; // Base sigma of Gaussian function for resampling
const int INT_NUMSUBPIXELS = 64;  // Number of subpixel locations for pre-calculated kernel weights


/**
 * Gets the Gaussian weight at dfX location.
 *
 * \param [in] dfX   Value at which to calculate the weight.
 * \param [in] fSigma The standard deviation of Gaussian function.
 */
double GetGaussianWeight(const double dfX, const float fSigma) {
  if (0.0 == dfX)
    return 1.0;

  const double dfX2 = dfX * dfX;
  const double dfSigma2 = 2.0 * fSigma * fSigma;

  return exp(-dfX2 / dfSigma2);
}

/**
 * Generates the kernel weights and stores them in ResampleInfo
 *
 * The kernel weights are organized as follows:     
 *
 * Gaussian uses a weighted sum of the nWndNumCols/nWndNumRows samples  
 * around a certain subpixel location to interpolate the value at that   
 * subpixel location.  There are 64 subpixel locations. The set of      
 * weights that are used to interpolate the samples at subpixel location
 * are based on the gaussian function. The distance of each sample      
 * from the subpixel location is calculated and the weight is simply    
 * the normalized exp( -x^2 / sigma^2) of that distance.      
 *
 * The kernel weights are stored with the first nWndNumCols representing
 * the weights at subpixel location 0, the next nWndNumCols are at      
 * subpixel location 1, and so on, until subpixel location 63. There    
 * are thus nWndNumCols * 64 kernel weights for col direction.          
 * The nWndNumRows * 64 kernel weights for row direction follow behind. 
 *
 * \param [in] nWndNumCols Number of columns in the gaussian window.
 * \param [in] nWndNumRows Number of rows in the gaussian window.
 * \param [in] fSigmaCol Sigma of Gaussian function in col direction.
 * \param [in] fSigmaRow Sigma of Gaussian function in row direction.
 * \param [out] vdfKernelWeights The generated kernel weights are stored in here.
 */
void GenerateGaussianKernel(const int nWndNumCols, const int nWndNumRows,
  const float fSigmaCol, const float fSigmaRow, std::vector<double>& vdfKernelWeights) {
  vdfKernelWeights.clear();

  // @note that there will actually by 65 sets of weights, the last one is
  // for subpixel locations very close to next whole pixel (e.g. 0.999999).

  // total number of sample points
  const size_t nColWeights = nWndNumCols * (INT_NUMSUBPIXELS + 1);
  const size_t nTotWeights = nColWeights + nWndNumRows * (INT_NUMSUBPIXELS + 1);

  // allocate the points.
  vdfKernelWeights.resize(nTotWeights);

  // the increment interval between subpixel locations.
  const double dfSpacing = 1.0 / double(INT_NUMSUBPIXELS);

  int i, j;
  double dfX, dfSum, dfActSum;

  /*      Kernel weights for col.                                               */
  // the nearest smaller pixel location to the subpixel location.
  const int nWndColOff = (nWndNumCols - 1) / 2;

  // go through all the 64 subpixel locations and calculate the weights
  size_t nKernelOff = 0;
  double dfSubPixel = 0;
  for (i = 0; i <= INT_NUMSUBPIXELS;
    ++i, dfSubPixel += dfSpacing, nKernelOff += nWndNumCols) {
    dfSum = 0.0; // reset the sum of weights.
    for (j = 0; j < nWndNumCols; ++j) {
      // center the locations of all samples about the subpixel location
      dfX = double(j) - double(nWndColOff + dfSubPixel);

      // get the weight from the gaussian window.
      vdfKernelWeights[nKernelOff + j] =
        GetGaussianWeight(dfX, fSigmaCol);
      dfSum += vdfKernelWeights[nKernelOff + j];
    }

    // normalize the sum of the weights to 1. this preserves the DC /
    // constant value in flat areas.
    dfActSum = 0.0;
    for (j = 0; j < nWndNumCols; j++) {
      vdfKernelWeights[nKernelOff + j] /= dfSum;
      dfActSum += vdfKernelWeights[nKernelOff + j];
    }

    vdfKernelWeights[nKernelOff + nWndColOff] += (1.0 - dfActSum);
  }

  /*      Kernel weights for row.                                               */
  // the nearest smaller pixel location to the subpixel location.
  const int nWndRowOff = (nWndNumRows - 1) / 2;

  // go through all the 64 subpixel locations and calculate the weights
  nKernelOff = nColWeights;
  dfSubPixel = 0;
  for (i = 0; i <= INT_NUMSUBPIXELS;
    ++i, dfSubPixel += dfSpacing, nKernelOff += nWndNumRows) {
    dfSum = 0.0; // reset the sum of weights.
    for (j = 0; j < nWndNumRows; ++j) {
      // center the locations of all samples about the subpixel location
      dfX = double(j) - double(nWndRowOff + dfSubPixel);

      // get the weight from the gaussian window.
      vdfKernelWeights[nKernelOff + j] =
        GetGaussianWeight(dfX, fSigmaRow);
      dfSum += vdfKernelWeights[nKernelOff + j];
    }

    // normalize the sum of the weights to 1. this preserves the DC /
    // constant value in flat areas.
    dfActSum = 0.0;
    for (j = 0; j < nWndNumRows; j++) {
      vdfKernelWeights[nKernelOff + j] /= dfSum;
      dfActSum += vdfKernelWeights[nKernelOff + j];
    }

    vdfKernelWeights[nKernelOff + nWndRowOff] += (1.0 - dfActSum);
  }
}


TEST(resample, GaussianKernel) {
  std::cout << std::fixed;
  std::cout << std::setprecision(12);

  // determine the col sigma for downsampling
  float fDsFactorCol = 9.5;
  const float fDsCol = std::max(1.1f, fDsFactorCol);
  const float fSigmaCol = FLT_BASESIGMA * sqrt(float(fDsCol * fDsCol - 1.0f));
  const int nWndNumCols = std::max(1, int(floor(fSigmaCol * 3 + 0.5f))) * 2;

  // determine the row sigma for downsampling
  float fDsFactorRow = 9.5;
  const float fDsRow = std::max(1.1f, fDsFactorRow);
  const float fSigmaRow = FLT_BASESIGMA * sqrt(float(fDsRow * fDsRow - 1.0f));
  const int nWndNumRows = std::max(1, int(floor(fSigmaRow * 3 + 0.5f))) * 2;

  std::vector<double> vdfKernelWeights;
  GenerateGaussianKernel(nWndNumCols, nWndNumRows, fSigmaCol, fSigmaRow, vdfKernelWeights);

  std::cout << vdfKernelWeights.size();

  int idx = 0;
  for (int i = 0; i < INT_NUMSUBPIXELS + 1; ++i) {
    std::cout << '[' << vdfKernelWeights[idx++];
    for (int j = 1; j < nWndNumCols; ++j) {
      std::cout << ", " << vdfKernelWeights[idx++];
    }
    std::cout << "]," << std::endl;
  }
}
