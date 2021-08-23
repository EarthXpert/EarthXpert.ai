/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalfit.h"
#include "core/exceptions/exexcept.h"
#include "core/math/mutils.h"

using namespace earthxpert;

RationalFit::RationalFit(const int coefGroup) : mCoefGroup(coefGroup) {
  Initialize();
}

void RationalFit::Initialize() {
  SetCoefGroup(mCoefGroup);
}

/**
 *  \brief Set flags indicating coefficients group to be estimated.
 *
 *  \param [in] coefGroup flag a bitwise-OR combination of selected groups
 */
void RationalFit::SetCoefGroup(const int coefGroup) {
  mCoefFlags.set(); // default using all polynomial coefficients

  int flag{ coefGroup };
  if (coefGroup & kDLT) {
    flag = kEqDenom | kNoQuad | kNoCubic;
  }
  if (coefGroup & kPrj2D) {
    flag = kEqDenom | kNoQuad | kNoCubic | kNoHgt;
  }

  mEqDenom = flag & kEqDenom;
  std::vector<rpc> noIdxs;

  if (flag & kNoQuad) {  // No quadratic terms
    for (auto idx = rpc::eLP; idx <= rpc::eHH; ++idx) {
      noIdxs.push_back(idx);
    }
  }

  if (flag & kNoCubic) {  // No cubic terms
    for (auto idx = rpc::ePLH; idx <= rpc::eHHH; ++idx) {
      noIdxs.push_back(idx);
    }
  }

  if (flag & kNoHgt) {  // No height terms: single layer for ortho
    noIdxs.insert(noIdxs.cbegin(), { rpc::eH, rpc::eLH, rpc::ePH,
      rpc::eHH, rpc::ePLH, rpc::eLHH, rpc::ePHH, rpc::eHHH });
  }

  std::sort(noIdxs.begin(), noIdxs.end());
  std::erase(std::unique(noIdxs.begin(), noIdxs.end()), noIdxs.end());
  
  for (const auto idx : noIdxs) {
    mCoefFlags.set(idx, false);
  }

  mNumCoefs = int(mCoefFlags.count());  // number of non-masked coefficients
  mNumUnks = mEqDenom ? (mNumCoefs + (mNumCoefs - 1) / 2) : (mNumCoefs * 2 - 1);
}

/**
 *  \brief specify normalization factors (offsets + scales),                            
 *  which are desired and useful in certain situations (stereo).                                                  
 */
void RationalFit::SetNormalFactors(const std::array<double, NumFact>& normalFators) {

}

void RationalFit::GetNormalFactors(std::array<double, NumFact>& normalFators) const {

}

/**
 *  \brief Set the prior weights used to constrain polynomial coefficients.
 *
 *  For most sensors, the amount of contribution of the terms to
 *  the imaging geometry should be going down from offset 
 *  to linear to quadratic and through to cubic terms.
 * 
 *  By experience, it is often desirable to set a small prior weight
 *  on quadratic and cubic terms when performing coefficient estimation. This
 *  keeps them from getting large values, resulting in a situation where the
 *  high-order polynomial terms and the low-order polynomial terms cancel each
 *  other to get a stable solution. 
 *
 * \param [in] wtOff weight of offset terms
 * \param [in] wtLin weight of linear terms
 * \param [in] wtQuad weight of quadratic terms
 * \param [in] wtCub weight of cubic terms
 *
 * \except thrown if any paraemter is negative
 */
void RationalFit::SetCoefWeights(double wtOff,
  double wtLin, double wtQuad, double wtCub) {
  auto coefWt = [](const double wt, const std::string& var) {
    if (0.0 > wt) 
      throw std::invalid_argument(var);
    return wt;
  };

  mWtOff = coefWt(wtOff, "wtOff");
  mWtLin = coefWt(wtLin, "wtLin");
  mWtQuad = coefWt(wtQuad, "wtQuad");
  mWtCub = coefWt(wtCub, "wtCub");
}

/**
 * Get weights used to constrain polynomial coefficients
 *
 * \param [out] wtOff weight for offset terms
 * \param [out] wtLin weight for linear terms
 * \param [out] wtQuad weight for quadratic terms
 * \param [out] wtCub weight for cubic terms
 */
void RationalFit::GetCoefWeights(double& wtOff,
  double& wtLin, double& wtQuad, double& wtCub) const {
  wtOff = mWtOff;
  wtLin = mWtLin;
  wtQuad = mWtQuad;
  wtCub = mWtCub;
}

/**
 *  \brief calculate coefficients from given points.
 *  
 *  \param [in] images an array of image grid points. 
 *    The line/pixel coordinate system has the origin 
 *    (0.0, 0.0) corresponding to the upper-left corner of 
 *    the upper-left pixel, i.e., the pixel centre of the 
 *    upper-left pixel is at (0.5, 0.5).
 *  \param [in] grounds an array of the corresponding ground cube points
 *  \param [in] lineNumCoef numerator polynomial of line formulae
 *  \param [in] lineDenCoef denominator polynomial of line formulae
 *  \param [in] sampNumCoef numerator polynomial of sample formulae
 *  \param [in] sampDenCoef denominator polynomial of sample formulae
 */
void RationalFit::CalcCoefficients(
  const std::vector<Point2f>& images, const std::vector<Point3d>& grounds,
  std::array<double, rpc::NumCoef>& lineNumCoef,
  std::array<double, rpc::NumCoef>& lineDenCoef,
  std::array<double, rpc::NumCoef>& sampNumCoef,
  std::array<double, rpc::NumCoef>& sampDenCoef) {
  if (images.size() != grounds.size()) {
    throw std::invalid_argument(_("image grid and ground cube are different in size."));
  }

  // Convert row/col coordinates from raster CS to indices.
  // The RPCs internally uses the pixel centre as the origin.                                                         */
  std::vector<Point2d> imgs;
  std::transform(images.cbegin(), images.cend(), 
    std::back_inserter(imgs), [](const Point2f& img) -> Point2d { 
      return (img - 0.5f); });

  CalcNormalFactors(imgs, grounds);

  if (mEqDenom) {
    LsqEstimateCommonDenom(oLat, oLon, oHgt, oRowRpc, oColRpc,
      adfOff, adfScale, poCoordSys, poModel);
  }
  else {
    LsqEstimate(oLat, oLon, oHgt, oRowRpc, oColRpc, adfOff,
      adfScale, poCoordSys, poModel);
  }
}

/**
 *  \brief calculate the normalization factors used for scaling values from
 *  (row,col,lat,lon,hgt) to apply to the polynomial coefficients. 
 *  If not normalized, the estimation could have numerical difficulties 
 *  for 3rd-order polynomials when the coordinate values are large.
 *  
 *  \param [in] images an array of image grid points.
 *  \param [in] grounds an array of the corresponding ground cube points
 */
void
RationalFit::CalcNormalFactors(const std::vector<Point2d>& images,
  const std::vector<Point3d>& grounds) {
  const int n = int(images.size());
  if (NumOffScl == int(mOffFlags.count()) &&
      NumOffScl == int(mSclFlags.count())) {
    return;
  }
  std::vector<double> arr(n);

  auto truncateOffDigits = [](const double off, const int nunDigits) -> double {
    if (0 > nunDigits) return val;
    const double pd = pow(10.0, nunDigits);
    return std::round(off * pd) / pd;
  };

  auto truncateSclDigits = [](const double scl, const int nunDigits) -> double {
    if (0 > nunDigits) return val;
    const double pd = pow(10.0, nunDigits);
    const double sclN = std::round(scl * pd) / pd;
    return (0.0 != sclN) ? sclN : pow(10.0, -nunDigits);
  };

  auto offScale = [](const std::vector<double>& arr, 
    const int offDigits, const int sclDigits) -> std::pair<double, double> {
    const auto sum = std::accumulate(arr.cbegin(), arr.cend(), 0.0);
    auto off = truncateOffDigits(sum / double(arr.size(), offDigits);

    auto [min, max] = std::minmax_element(arr.cbegin(), arr.cend());
    auto scl = truncateSclDigits(std::max(off - *min, *max - off), sclDigits);

    return std::make_pair(off, scl);
  };

  std::transform(images.cbegin(), images.cend(), arr.begin(), 
    [](const Point2f& img) -> double { return img.y; });
  auto [offLine, sclLine] = offScale(arr, mOffDigits[0], mSclDigits[0]);
}

/******************************************************************************/
/*      GetPriorWeightArray()                                                 */
/******************************************************************************/
/**
 * Get an array of prior weights for the 20 coefficients of
 * the polynomial.
 *
 * \param [out] padfWeights array of weights
 */
void
RationalFit::GetPriorWeightArray(double* const padfWeights) const
{
  memset(padfWeights, 0, 20 * sizeof(double));

  /* -------------------------------------------------------------------------- */
  /*      Set them initially based on the provided weights.                     */
  /* -------------------------------------------------------------------------- */
  padfWeights[RpcMathModel::OFF] = mWtOff;
  padfWeights[RpcMathModel::L] = mWtLin;
  padfWeights[RpcMathModel::P] = mWtLin;
  padfWeights[RpcMathModel::H] = mWtLin;
  padfWeights[RpcMathModel::LP] = mWtQuad;
  padfWeights[RpcMathModel::LH] = mWtQuad;
  padfWeights[RpcMathModel::PH] = mWtQuad;
  padfWeights[RpcMathModel::LL] = mWtQuad;
  padfWeights[RpcMathModel::PP] = mWtQuad;
  padfWeights[RpcMathModel::HH] = mWtQuad;
  padfWeights[RpcMathModel::PLH] = mWtCub;
  padfWeights[RpcMathModel::LLL] = mWtCub;
  padfWeights[RpcMathModel::LPP] = mWtCub;
  padfWeights[RpcMathModel::LHH] = mWtCub;
  padfWeights[RpcMathModel::LLP] = mWtCub;
  padfWeights[RpcMathModel::PPP] = mWtCub;
  padfWeights[RpcMathModel::PHH] = mWtCub;
  padfWeights[RpcMathModel::LLH] = mWtCub;
  padfWeights[RpcMathModel::PPH] = mWtCub;
  padfWeights[RpcMathModel::HHH] = mWtCub;

  /* -------------------------------------------------------------------------- */
  /*      Now go through them again.  It's possible that the user               */
  /*      has defined a zero prior weight for some coefficients but             */
  /*      then completely disabled them by choosing a model flag like           */
  /*      NO_HT_TERMS.  In those cases, we want to set a default value          */
  /*      so that we don't have to go through and explicitly remove             */
  /*      these parameters from the normal equations.                           */
  /* -------------------------------------------------------------------------- */
  for (int i = 0; i < 20; ++i)
  {
    if (false == mabCoefEstFlag[i])
    {
      padfWeights[i] = 1.0;
    }
  }

  return;
}// GetPriorWeightArray

/******************************************************************************/
/*          CalcLPHTerms()                                                    */
/******************************************************************************/
/**
 * Calculate the combinations of L, P, and H that go with the Rational
 * Function coefficients.
 *
 * \param [in] dfL normalized Lambda (longitude)
 * \param [in] dfP normalized Phi (latitude)
 * \param [in] dfH normalized Height
 * \param [in] padfCoefMask array of values that indicate which coefficients are masked out
 * \param [in] padfLPHTerms masked array of L, P, H combinations
 */
void
RationalFit::CalcLPHTerms(const double& dfL,
  const double& dfP,
  const double& dfH,
  const double* const padfCoefMask,
  double* const padfLPHTerms) const
{
  padfLPHTerms[RpcMathModel::OFF] = 1.0 * padfCoefMask[RpcMathModel::OFF];
  padfLPHTerms[RpcMathModel::L] = dfL * padfCoefMask[RpcMathModel::L];
  padfLPHTerms[RpcMathModel::P] = dfP * padfCoefMask[RpcMathModel::P];
  padfLPHTerms[RpcMathModel::H] = dfH * padfCoefMask[RpcMathModel::H];
  padfLPHTerms[RpcMathModel::LP] = dfL * dfP * padfCoefMask[RpcMathModel::LP];
  padfLPHTerms[RpcMathModel::LH] = dfL * dfH * padfCoefMask[RpcMathModel::LH];
  padfLPHTerms[RpcMathModel::PH] = dfP * dfH * padfCoefMask[RpcMathModel::PH];
  padfLPHTerms[RpcMathModel::LL] = dfL * dfL * padfCoefMask[RpcMathModel::LL];
  padfLPHTerms[RpcMathModel::PP] = dfP * dfP * padfCoefMask[RpcMathModel::PP];
  padfLPHTerms[RpcMathModel::HH] = dfH * dfH * padfCoefMask[RpcMathModel::HH];
  padfLPHTerms[RpcMathModel::PLH] = dfP * dfL * dfH * padfCoefMask[RpcMathModel::PLH];
  padfLPHTerms[RpcMathModel::LLL] = dfL * dfL * dfL * padfCoefMask[RpcMathModel::LLL];
  padfLPHTerms[RpcMathModel::LPP] = dfL * dfP * dfP * padfCoefMask[RpcMathModel::LPP];
  padfLPHTerms[RpcMathModel::LHH] = dfL * dfH * dfH * padfCoefMask[RpcMathModel::LHH];
  padfLPHTerms[RpcMathModel::LLP] = dfL * dfL * dfP * padfCoefMask[RpcMathModel::LLP];
  padfLPHTerms[RpcMathModel::PPP] = dfP * dfP * dfP * padfCoefMask[RpcMathModel::PPP];
  padfLPHTerms[RpcMathModel::PHH] = dfP * dfH * dfH * padfCoefMask[RpcMathModel::PHH];
  padfLPHTerms[RpcMathModel::LLH] = dfL * dfL * dfH * padfCoefMask[RpcMathModel::LLH];
  padfLPHTerms[RpcMathModel::PPH] = dfP * dfP * dfH * padfCoefMask[RpcMathModel::PPH];
  padfLPHTerms[RpcMathModel::HHH] = dfH * dfH * dfH * padfCoefMask[RpcMathModel::HHH];

  return;
}// CalcLPHTerms


/******************************************************************************/
/*      CreateRpcMathModel()                                                       */
/******************************************************************************/
/**
 * Create a new Rational Function Model object and populate it.
 *
 * \param [in] padfOff array of offset values for normalization
 * \param [in] padfScale array of scale values for normalization
 * \param [in] padfLineNumCoef array of line numerator coefficients
 * \param [in] padfLineDenCoef array of line denominator coefficients
 * \param [in] padfSampNumCoef array of sample numerator coefficients
 * \param [in] padfSampDenCoef array of sample denominator coefficients
 * \param [in] poCoordSys ground coordinate system of the model
 * @param[in,out] poModel RpcMathModel object to populate
 */
void
RationalFit::CreateRpcMathModel(const double* const padfOff,
  const double* const padfScale,
  const double* const padfLineNumCoef,
  const double* const padfLineDenCoef,
  const double* const padfSampNumCoef,
  const double* const padfSampDenCoef,
  pci::ogc::cs::ICRS* poCoordSys,
  RpcMathModel*& poModel) const
{
  double dfLatOff = padfOff[RationalFit::IDX_LAT];
  double dfLongOff = padfOff[RationalFit::IDX_LON];
  double dfHeightOff = padfOff[RationalFit::IDX_HGT];
  double dfLineOff = padfOff[RationalFit::IDX_ROW];
  double dfSampOff = padfOff[RationalFit::IDX_COL];
  double dfLatScale = padfScale[RationalFit::IDX_LAT];
  double dfLongScale = padfScale[RationalFit::IDX_LON];
  double dfHeightScale = padfScale[RationalFit::IDX_HGT];
  double dfLineScale = padfScale[RationalFit::IDX_ROW];
  double dfSampScale = padfScale[RationalFit::IDX_COL];

  poModel = new RpcMathModel(dfLineOff, dfSampOff, dfLatOff, dfLongOff, dfHeightOff,
    dfLineScale, dfSampScale, dfLatScale, dfLongScale, dfHeightScale,
    padfLineNumCoef, padfLineDenCoef, padfSampNumCoef, padfSampDenCoef,
    poCoordSys);

  return;
}// CreateRpcMathModel

/******************************************************************************/
/*      CalcRms()                                                             */
/******************************************************************************/
/**
 * Compute RMS of the residual fit errors by comparison with
 * a provided point grid derived from the physical model.
 *
 * \param [in] oLat vector of latitudes
 * \param [in] oLon vector of longitudes
 * \param [in] oHgt vector of heights
 * \param [in] oRow vector of row (line) coordinates
 * \param [in] oCol vector of column (sample) coordinates
 * \param [in] nNumEstParms number of parameters that were used in order
 *            to estimate the RPC coefficients
 * \param [in] poModel the Rational Function Model
 * \param [out] dfRmsR row RMS
 * \param [out] dfRmsC column RMS
 *
 */
void
RationalFit::CalcRms(const std::vector<double>& oLat,
  const std::vector<double>& oLon,
  const std::vector<double>& oHgt,
  const std::vector<double>& oRow,
  const std::vector<double>& oCol,
  const RpcMathModel* poModel,
  const int& nNumEstParms,
  double& dfRmsR,
  double& dfRmsC) const
{

  if (true == mbReportFlag)
  {
    mcADDINFO("RPC Fit Statistics\n");
  }

  int iNum = (int)oLat.size();
  int i;

  dfRmsR = 0.0;
  dfRmsC = 0.0;
  double dfCpred;
  double dfRpred;
  double dfCerr;
  double dfRerr;

  double dfCerrMax = 0.0;
  double dfCerrMaxCol = 0.0;
  double dfCerrMaxRow = 0.0;
  double dfCerrMaxHgt = 0.0;

  double dfRerrMax = 0.0;
  double dfRerrMaxCol = 0.0;
  double dfRerrMaxRow = 0.0;
  double dfRerrMaxHgt = 0.0;

  int iInc = 1;
  int iNumRmsPts = 0;

  for (i = 0, iNumRmsPts = 0; i < iNum; i += iInc)
  {
    poModel->GroundToImage(oLon[i], oLat[i], oHgt[i], dfCpred, dfRpred);
    dfRerr = dfRpred - oRow[i];
    dfCerr = dfCpred - oCol[i];

    dfRmsR += dfRerr * dfRerr;
    dfRmsC += dfCerr * dfCerr;
    iNumRmsPts++;

    if (0 != i)
    {
      if (dfCerrMax < fabs(dfCerr))
      {
        dfCerrMax = fabs(dfCerr);
        dfCerrMaxCol = oCol[i];
        dfCerrMaxRow = oRow[i];
        dfCerrMaxHgt = oHgt[i];
      }
      if (dfRerrMax < fabs(dfRerr))
      {
        dfRerrMax = fabs(dfRerr);
        dfRerrMaxCol = oCol[i];
        dfRerrMaxRow = oRow[i];
        dfRerrMaxHgt = oHgt[i];
      }
    }
    else
    {
      dfCerrMax = fabs(dfCerr);
      dfCerrMaxCol = oCol[i];
      dfCerrMaxRow = oRow[i];
      dfCerrMaxHgt = oHgt[i];

      dfRerrMax = fabs(dfRerr);
      dfRerrMaxCol = oCol[i];
      dfRerrMaxRow = oRow[i];
      dfRerrMaxHgt = oHgt[i];
    }
  }// i loop over observations

  // Compute degrees of freedom
  double dfDof = (double)(iNumRmsPts - nNumEstParms);
  if (dfDof <= 0.0) dfDof = 1.0;

  dfRmsR = sqrt(dfRmsR / dfDof);
  dfRmsC = sqrt(dfRmsC / dfDof);

  // Report results
  if (true == mbReportFlag)
  {
    if (0.001 < dfRmsC)
    {
      mcADDINFO(FormatString("Col RMS error   : %.3lf pixels\n", dfRmsC));
    }
    else
    {
      mcADDINFO(FormatString("Col RMS error   : %.e pixels\n", dfRmsC));
    }

    if (0.001 < dfRmsR)
    {
      mcADDINFO(FormatString("Row RMS error   : %.3lf pixels\n", dfRmsR));
    }
    else
    {
      mcADDINFO(FormatString("Row RMS error   : %.e pixels\n", dfRmsR));
    }

    mcADDINFO(FormatString("Worst col error : %.3lf pixels at (row, col, hgt): %5.0lf, %5.0lf, %5.0lf\n",
      dfCerrMax, dfCerrMaxRow, dfCerrMaxCol, dfCerrMaxHgt));

    mcADDINFO(FormatString("Worst row error : %.3lf pixels at (row, col, hgt): %5.0lf, %5.0lf, %5.0lf\n",
      dfRerrMax, dfRerrMaxRow, dfRerrMaxCol, dfRerrMaxHgt));
  }

  return;
}// CalcRms


/******************************************************************************/
/*      LsqEstimate()                                                         */
/******************************************************************************/
/**
 * Estimate the Rational Function coefficients.
 *
 * \param [in] oLat vector of longitudes
 * \param [in] oLon vector of latitudes
 * \param [in] oHgt vector of heights
 * \param [in] oRow vector of row (line) coordinates
 * \param [in] oCol vector of column (sample) coordinates
 * \param [in] padfOff array of offset values (Lat, Lon, Hgt, Row, Col)
 * \param [in] padfScale array of scale values (Lat, Lon, Hgt, Row, Col)
 * \param [in] poCoordSys ground coordinate system of oLat, oLon, oHgt
 * \param [out] poModel populated Rational Function Model with
 *             estimated coefficients
 * @return 0 on success, non-zero otherwise
 * @exception pci::DataException thrown if convergence not achieved
 */
long
RationalFit::LsqEstimate(const std::vector<double>& oLat,
  const std::vector<double>& oLon,
  const std::vector<double>& oHgt,
  const std::vector<double>& oRow,
  const std::vector<double>& oCol,
  const double* const padfOff,
  const double* const padfScale,
  pci::ogc::cs::ICRS* poCoordSys,
  RpcMathModel*& poModel) const
{
  double adfPriorWeights[20];   // Prior weights for rational polynomial coefficients
  double adfCoefMask[20];       // Mask set to 0 for unused coefficients
  double adfLPH[20];            // The combinations of P, L, H that go with the coefficients.
  int i;                        // Loop counter
  int j;                        // Loop counter
  const int iNumParms = 39;     // Number of unknown parameters.  This doesn't make the
                                // routine completely general, but it makes it easier to port
                                // the code for the case where line/sample use the same denominator.
  int iNumObs = (int)oLat.size(); // Total number of observations.
  int iObs;                     // Loop counter over observations
  int iIter;                    // Loop counter over least squares iterations
  int iStatus;                  // General status variable
  //int iNumRhs = 1;              // Number of right-hand-sides for equation system solver

/* -------------------------------------------------------------------------- */
/*      Get a "coefficient mask" for the coefficients.  If a                  */
/*      coefficient has been turned off, the coefficient mask                 */
/*      has a zero and the partial derivative with respect to this            */
/*      coefficient will be multiplied by zero to deactivate it.              */
/* -------------------------------------------------------------------------- */
  GetCoefMask(adfCoefMask);

  /* -------------------------------------------------------------------------- */
  /*      Get prior weights for the coefficients.  If some coefficients         */
  /*      have been deactivated, we have to make sure that the                  */
  /*      prior weight is nonzero to avoid a singularity in the                 */
  /*      equation system.                                                      */
  /* -------------------------------------------------------------------------- */
  GetPriorWeightArray(adfPriorWeights);

  /* -------------------------------------------------------------------------- */
  /*      Define the full vector of prior weights.  It has 39                   */
  /*      elements -- 20 for the numerator coefficients and 19                  */
  /*      for the denominator coefficients. The first denominator               */
  /*      coefficient is left at 1.0 to provide scale for the                   */
  /*      rational function.                                                    */
  /* -------------------------------------------------------------------------- */
  double adfPx[iNumParms];    // Array of prior weights.
  for (i = 0; i < 20; ++i)
  {
    // Numerator coefficients
    adfPx[i] = adfPriorWeights[i];

    // Denominator coefficients
    if (0 < i)
    {
      adfPx[19 + i] = adfPriorWeights[i];
    }
  }// i loop over the different coefficients

/* -------------------------------------------------------------------------- */
/*      Define the matrices and vectors which will be used in the             */
/*      least squares estimation                                              */
/* -------------------------------------------------------------------------- */
  double adfAs[iNumParms];     // dF/dx for a single observation (sample)
  double adfAl[iNumParms];     // dF/dx for a single observation (line)

  double adfUs[iNumParms];     // At*w for sample
  double dfWs;                 // misclosure for a single observation (sample)

  double adfUl[iNumParms];     // At*w for line
  double dfWl;                 // misclosure for a single observation (line)

  double adfXs[iNumParms];     // Rational Function coefficient parameters (sample)
  double adfDxs[iNumParms];    // Rational Function coefficient updates (sample)

  double adfXl[iNumParms];     // Rational Function coefficient parameters (line)
  double adfDxl[iNumParms];    // Rational Function coefficient update (line)

  int iMatNumEl = iNumParms * iNumParms;        // Number of elements in the normal equations
  double** ppadfNs = 0;       // Normal equations for sample
  double** ppadfNl = 0;       // Normal equations for line
  double** ppadfNsfact = 0;   // Cholesky inverse of sample normals
  double** ppadfNlfact = 0;   // Cholesky inverse of line normals

  AllocMat(ppadfNs, iNumParms, iNumParms);
  AllocMat(ppadfNl, iNumParms, iNumParms);
  AllocMat(ppadfNsfact, iNumParms, iNumParms);
  AllocMat(ppadfNlfact, iNumParms, iNumParms);

  // Values used for intermediate calculations
  double P;                  // Normalized latitude (phi)
  double L;                  // Normalized longitude (lambda)
  double H;                  // Normalized height
  double C;                  // Normalized sample
  double R;                  // Normalized line
  double dfLatOff = padfOff[RationalFit::IDX_LAT];   // Make the offsets/scales a bit more readable
  double dfLongOff = padfOff[RationalFit::IDX_LON];
  double dfHeightOff = padfOff[RationalFit::IDX_HGT];
  double dfLineOff = padfOff[RationalFit::IDX_ROW];
  double dfSampOff = padfOff[RationalFit::IDX_COL];
  double dfLatScale = padfScale[RationalFit::IDX_LAT];
  double dfLongScale = padfScale[RationalFit::IDX_LON];
  double dfHeightScale = padfScale[RationalFit::IDX_HGT];
  double dfLineScale = padfScale[RationalFit::IDX_ROW];
  double dfSampScale = padfScale[RationalFit::IDX_COL];

  /* -------------------------------------------------------------------------- */
  /*      Populate the normal equations and misclosure vectors                  */
  /*      one observation at a time.                                            */
  /* -------------------------------------------------------------------------- */
      // First, initialize the items that get updated.
  memset(adfAs, 0, iNumParms * sizeof(double));
  memset(ppadfNs[0], 0, iMatNumEl * sizeof(double));
  memset(adfUs, 0, iNumParms * sizeof(double));
  memset(adfXs, 0, iNumParms * sizeof(double));

  memset(adfAl, 0, iNumParms * sizeof(double));
  memset(ppadfNl[0], 0, iMatNumEl * sizeof(double));
  memset(adfUl, 0, iNumParms * sizeof(double));
  memset(adfXl, 0, iNumParms * sizeof(double));


  // Add the observations
  for (iObs = 0; iObs < iNumObs; ++iObs)
  {
    // Compute normalized quantities.
    P = (oLat[iObs] - dfLatOff) / dfLatScale;
    L = (oLon[iObs] - dfLongOff) / dfLongScale;
    H = (oHgt[iObs] - dfHeightOff) / dfHeightScale;
    R = (oRow[iObs] - dfLineOff) / dfLineScale;
    C = (oCol[iObs] - dfSampOff) / dfSampScale;

    // Compute factors to go with the Rational Function coefficients
    CalcLPHTerms(L, P, H, adfCoefMask, adfLPH);

    // Compute partial derivatives.
    for (i = 0; i < 20; ++i)
    {
      // Numerator coefficients
      adfAs[i] = -adfLPH[i];
      adfAl[i] = -adfLPH[i];

      // Denominator coefficients
      if (0 < i)
      {
        adfAs[i + 19] = C * adfLPH[i];
        adfAl[i + 19] = R * adfLPH[i];
      }//
    }// i loop over terms

    // Compute AtA and add to normals.
    // AtA is symmetric, so we don't calculate the lower-triangular
    // portion yet.  We'll fill it in later.
    for (i = 0; i < iNumParms; ++i)
    {
      for (j = i; j < iNumParms; ++j)
      {
        ppadfNs[i][j] += adfAs[i] * adfAs[j];
        ppadfNl[i][j] += adfAl[i] * adfAl[j];
      }// j loop over columns in N
    }// i loop over rows in N


   // Compute misclosures
    dfWs = C;  // This corresponds to C * DenCoef[0], which is always 1.
    dfWl = R;
    for (i = 0; i < iNumParms; ++i)
    {
      dfWs += adfXs[i] * adfAs[i];
      dfWl += adfXl[i] * adfAl[i];
    }

    // Compute Atw and add to U
    for (i = 0; i < iNumParms; ++i)
    {
      adfUs[i] += adfAs[i] * dfWs;
      adfUl[i] += adfAl[i] * dfWl;
    }
  }// iObs loop over observations to populate normal equations.

  // Populate the lower triangle of the normal equations
  for (i = 0; i < iNumParms; ++i)
  {
    for (j = 0; j < i; ++j)
    {
      ppadfNs[i][j] = ppadfNs[j][i];
      ppadfNl[i][j] = ppadfNl[j][i];
    }// j loop over columns in N
  }// i loop over rows in N

/* -------------------------------------------------------------------------- */
/*      Now add the prior weights for the coefficients.                       */
/* -------------------------------------------------------------------------- */
  for (i = 0; i < iNumParms; ++i)
  {
    ppadfNs[i][i] += adfPx[i];
    ppadfNl[i][i] += adfPx[i];
    adfUs[i] += adfPx[i] * adfXs[i];
    adfUl[i] += adfPx[i] * adfXl[i];
  }


  /* -------------------------------------------------------------------------- */
  /*      Factor the normal equations                                           */
  /* -------------------------------------------------------------------------- */

      // Column
  memcpy(ppadfNsfact[0], ppadfNs[0], iMatNumEl * sizeof(double));
  iStatus = mat_inv(ppadfNsfact, iNumParms, true);
  if (0 != iStatus)
  {
    throw pci::DataException(FormatString("Coefficient solver (Column): Matrix invert failed at parameter %d", (int)iStatus),
      LOCATION);
  }

  // Row
  memcpy(ppadfNlfact[0], ppadfNl[0], iMatNumEl * sizeof(double));
  iStatus = mat_inv(ppadfNlfact, iNumParms, true);
  if (0 != iStatus)
  {
    throw pci::DataException(FormatString("Coefficient solver (Row): Matrix invert failed at parameter %d", (int)iStatus),
      LOCATION);
  }

  /* -------------------------------------------------------------------------- */
  /*      Solve for corrections to parameters and apply them                    */
  /* -------------------------------------------------------------------------- */
      // Column
  MatVecMult(ppadfNsfact, adfUs, iNumParms, iNumParms, adfDxs);

  // Row
  MatVecMult(ppadfNlfact, adfUl, iNumParms, iNumParms, adfDxl);

  for (i = 0; i < iNumParms; ++i)
  {
    adfXs[i] -= adfDxs[i];
    adfXl[i] -= adfDxl[i];
  }

  /* -------------------------------------------------------------------------- */
  /*      Now we iterate until the parameters converge.  We                     */
  /*      don't need to repopulate the normal equations because the             */
  /*      parameters do not appear in the partial derivatives...we              */
  /*      just recompute the misclosures and parameter updates.                 */
  /* -------------------------------------------------------------------------- */
      // Check our current convergence level (amount of correction to parameters);
  double dfMaxCorrs = fabs(adfDxs[VecIdxMaxAbs(adfDxs, iNumParms)]);
  double dfMaxCorrl = fabs(adfDxl[VecIdxMaxAbs(adfDxl, iNumParms)]);
  double dfConvergence = std::max(dfMaxCorrs, dfMaxCorrl);

  if (true == mbReportFlag)
  {
    mcADDINFO(FormatString("Iteration 0: dx_max = %e\n", dfConvergence));
  }
  for (iIter = 1; iIter < mlMaxIter && dfConvergence > mdfConvergeTol; ++iIter)
  {
    memset(adfUs, 0, iNumParms * sizeof(double));
    memset(adfUl, 0, iNumParms * sizeof(double));

    /* -------------------------------------------------------------------------- */
    /*      Add observations to U vector one observation at a time.               */
    /* -------------------------------------------------------------------------- */
    for (iObs = 0; iObs < iNumObs; ++iObs)
    {
      // Compute normalized quantities.
      P = (oLat[iObs] - dfLatOff) / dfLatScale;
      L = (oLon[iObs] - dfLongOff) / dfLongScale;
      H = (oHgt[iObs] - dfHeightOff) / dfHeightScale;
      R = (oRow[iObs] - dfLineOff) / dfLineScale;
      C = (oCol[iObs] - dfSampOff) / dfSampScale;

      // Compute factors to go with the RPCs
      CalcLPHTerms(L, P, H, adfCoefMask, adfLPH);

      // Compute partial derivatives.
      for (i = 0; i < 20; ++i)
      {
        // Numerator coefficients
        adfAs[i] = -adfLPH[i];
        adfAl[i] = -adfLPH[i];

        // Denominator coefficients
        if (0 < i)
        {
          adfAs[i + 19] = C * adfLPH[i];
          adfAl[i + 19] = R * adfLPH[i];
        }//
      }// i loop over terms

      // Compute misclosures
      dfWs = C;  // This corresponds to the first denominator coefficient, which is always 1.
      dfWl = R;
      for (i = 0; i < iNumParms; ++i)
      {
        dfWs += adfXs[i] * adfAs[i];
        dfWl += adfXl[i] * adfAl[i];
      }

      // Compute Atw and add to U
      for (i = 0; i < iNumParms; ++i)
      {
        adfUs[i] += adfAs[i] * dfWs;
        adfUl[i] += adfAl[i] * dfWl;
      }
    }// iObs loop over observations

/* -------------------------------------------------------------------------- */
/*      Add prior constraints                                                 */
/* -------------------------------------------------------------------------- */
    for (i = 0; i < iNumParms; ++i)
    {
      adfUs[i] += adfPx[i] * adfXs[i];
      adfUl[i] += adfPx[i] * adfXl[i];
    }

    /* -------------------------------------------------------------------------- */
    /*      Solve for corrections to parameters and apply them                    */
    /* -------------------------------------------------------------------------- */
            // Column
    MatVecMult(ppadfNsfact, adfUs, iNumParms, iNumParms, adfDxs);

    // Row
    MatVecMult(ppadfNlfact, adfUl, iNumParms, iNumParms, adfDxl);

    for (i = 0; i < iNumParms; ++i)
    {
      adfXs[i] -= adfDxs[i];
      adfXl[i] -= adfDxl[i];
    }

    dfMaxCorrs = fabs(adfDxs[VecIdxMaxAbs(adfDxs, iNumParms)]);
    dfMaxCorrl = fabs(adfDxl[VecIdxMaxAbs(adfDxl, iNumParms)]);

    dfConvergence = std::max(dfMaxCorrs, dfMaxCorrl);

    if (true == mbReportFlag)
    {
      mcADDINFO(FormatString("Iteration %d: dx_max = %e\n", iIter,
        dfConvergence));
    }
  }// iter loop over iterations

/* -------------------------------------------------------------------------- */
/*      Throw an error if it didn't converge                                  */
/* -------------------------------------------------------------------------- */
  if (dfConvergence > mdfConvergeTol)
  {
    throw pci::DataException(FormatString("Error estimating rational function coefficients: convergence not achieve in %d iterations\nConvergence: %e\nTolerance: %e",
      mlMaxIter, dfConvergence, mdfConvergeTol),
      LOCATION);
  }

  if (true == mbReportFlag)
  {
    mcADDINFO(FormatString("Convergence achieved in %d iterations.\n\n", iIter));
  }

  /* -------------------------------------------------------------------------- */
  /*      Populate the Rational Function object                                 */
  /* -------------------------------------------------------------------------- */
  double adfLineNumCoef[20];
  double adfLineDenCoef[20];
  double adfSampNumCoef[20];
  double adfSampDenCoef[20];
  for (i = 0; i < 20; ++i)
  {
    adfLineNumCoef[i] = adfXl[i];
    adfSampNumCoef[i] = adfXs[i];
    if (0 < i)
    {
      adfLineDenCoef[i] = adfXl[i + 19];
      adfSampDenCoef[i] = adfXs[i + 19];
    }
    else
    {
      adfLineDenCoef[i] = 1.0;
      adfSampDenCoef[i] = 1.0;
    }
  }
  CreateRpcMathModel(padfOff, padfScale, adfLineNumCoef, adfLineDenCoef,
    adfSampNumCoef, adfSampDenCoef, poCoordSys, poModel);

  FreeMat(ppadfNs);
  FreeMat(ppadfNl);
  FreeMat(ppadfNsfact);
  FreeMat(ppadfNlfact);

  return 0;
}// LsqEstimate

/******************************************************************************/
/*      LsqEstimateCommonDenom()                                              */
/******************************************************************************/
/**
 * Estimate the Rational Function coefficients using common denominator terms.
 *
 * \param [in] oLat vector of longitudes
 * \param [in] oLon vector of latitudes
 * \param [in] oHgt vector of heights
 * \param [in] oRow vector of row (line) coordinates
 * \param [in] oCol vector of column (sample) coordinates
 * \param [in] padfOff array of offset values (Lat, Lon, Hgt, Row, Col)
 * \param [in] padfScale array of scale values (Lat, Lon, Hgt, Row, Col)
 * \param [in] poCoordSys ground coordinate system of oLat, oLon, oHgt
 * \param [out] poModel populated Rational Function Model with
 *             estimated coefficients
 * @return 0 on success, non-zero otherwise
 * @exception pci::DataException thrown if convergence not achieved or the
 * system cannot be solved
 */
long
RationalFit::LsqEstimateCommonDenom(const std::vector<double>& oLat,
  const std::vector<double>& oLon,
  const std::vector<double>& oHgt,
  const std::vector<double>& oRow,
  const std::vector<double>& oCol,
  const double* const padfOff,
  const double* const padfScale,
  pci::ogc::cs::ICRS* poCoordSys,
  RpcMathModel*& poModel) const
{

  double adfPriorWeights[20];   // Prior weights for rational polynomial coefficients
  double adfCoefMask[20];       // Mask set to 0 for unused coefficients
  double adfLPH[20];            // The combinations of P, L, H that go with the coefficients.
  int i;                        // Loop counter
  int j;                        // Loop counter
  const int iNumParms = 59;     // Number of unknown parameters.
  int iNumObs = (int)oLat.size(); // Number of observations
  int iObs;                     // Loop counter over observations
  int iIter;                    // Loop counter over iterations
  int iStatus;                  // General status variable

/* -------------------------------------------------------------------------- */
/*      Get a "coefficient mask" for the coefficients.  If a                  */
/*      coefficient has been turned off, the coefficient mask                 */
/*      has a zero and the partial derivative with respect to this            */
/*      coefficient will be multiplied by zero to deactivate it.              */
/* -------------------------------------------------------------------------- */
  GetCoefMask(adfCoefMask);

  /* -------------------------------------------------------------------------- */
  /*      Get prior weights for the coefficients.  If some coefficients         */
  /*      have been deactivated, we have to make sure that the                  */
  /*      prior weight is nonzero to avoid a singularity in the                 */
  /*      equation system.                                                      */
  /* -------------------------------------------------------------------------- */
  GetPriorWeightArray(adfPriorWeights);

  /* -------------------------------------------------------------------------- */
  /*      Define the full vector of prior weights.  It has 59                   */
  /*      elements -- 20 for the line numerator coefficients,                   */
  /*      20 for the sample numerator coefficients, and 19                      */
  /*      for the denominator coefficients. The first denominator               */
  /*      coefficient is left at 1.0 to provide scale for the                   */
  /*      rational function.                                                    */
  /* -------------------------------------------------------------------------- */
  double adfPx[iNumParms];    // Array of prior weights.
  for (i = 0; i < 20; ++i)
  {
    // Numerator coefficients
    adfPx[i] = adfPriorWeights[i];
    adfPx[20 + i] = adfPriorWeights[i];

    // Denominator coefficients
    if (0 < i)
    {
      adfPx[39 + i] = adfPriorWeights[i];
    }
  }// i loop over the different coefficients

/* -------------------------------------------------------------------------- */
/*      Define the matrices and vectors which will be used in the             */
/*      least squares estimation                                              */
/* -------------------------------------------------------------------------- */
  double adfAs[iNumParms];    // dF/dx for a single sample observation
  double adfAl[iNumParms];    // dF/dx for a single line observation
  double adfU[iNumParms];     // At*w
  double dfWs;                // Sample misclosure
  double dfWl;                // Line misclosure

  double adfX[iNumParms];     // Rational Function coefficient parameters
  double adfDx[iNumParms];    // Rational Function coefficient updates

  int iMatNumEl = iNumParms * iNumParms;        // Number of elements in the normal equations
  double** ppadfN = 0;        // Normal equations
  double** ppadfNfact = 0;    // Cholesky inverse of normals

  AllocMat(ppadfN, iNumParms, iNumParms);
  AllocMat(ppadfNfact, iNumParms, iNumParms);

  // Values used for intermediate calculations
  double P;                  // Normalized latitude (phi)
  double L;                  // Normalized longitude (lambda)
  double H;                  // Normalized height
  double C;                  // Normalized sample
  double R;                  // Normalized line
  double dfLatOff = padfOff[RationalFit::IDX_LAT];   // Make the offsets/scales a bit more readable
  double dfLongOff = padfOff[RationalFit::IDX_LON];
  double dfHeightOff = padfOff[RationalFit::IDX_HGT];
  double dfLineOff = padfOff[RationalFit::IDX_ROW];
  double dfSampOff = padfOff[RationalFit::IDX_COL];
  double dfLatScale = padfScale[RationalFit::IDX_LAT];
  double dfLongScale = padfScale[RationalFit::IDX_LON];
  double dfHeightScale = padfScale[RationalFit::IDX_HGT];
  double dfLineScale = padfScale[RationalFit::IDX_ROW];
  double dfSampScale = padfScale[RationalFit::IDX_COL];

  /* -------------------------------------------------------------------------- */
  /*      Populate the normal equations and misclosure vectors                  */
  /*      one observation at a time.                                            */
  /* -------------------------------------------------------------------------- */

      // Initialize values
  memset(adfAs, 0, iNumParms * sizeof(double));
  memset(adfAl, 0, iNumParms * sizeof(double));
  memset(ppadfN[0], 0, iMatNumEl * sizeof(double));
  memset(adfU, 0, iNumParms * sizeof(double));
  memset(adfX, 0, iNumParms * sizeof(double));

  // Add the observations
  for (iObs = 0; iObs < iNumObs; ++iObs)
  {
    // Compute normalized quantities.
    P = (oLat[iObs] - dfLatOff) / dfLatScale;
    L = (oLon[iObs] - dfLongOff) / dfLongScale;
    H = (oHgt[iObs] - dfHeightOff) / dfHeightScale;
    R = (oRow[iObs] - dfLineOff) / dfLineScale;
    C = (oCol[iObs] - dfSampOff) / dfSampScale;

    // Compute factors to go with the RPCs
    CalcLPHTerms(L, P, H, adfCoefMask, adfLPH);

    // Compute partial derivatives.
    for (i = 0; i < 20; ++i)
    {
      // Numerator coefficients
      adfAl[i] = -adfLPH[i];
      adfAs[i + 20] = -adfLPH[i];

      // Denominator coefficients
      if (0 < i)
      {
        adfAl[i + 39] = R * adfLPH[i];
        adfAs[i + 39] = C * adfLPH[i];
      }//
    }// i loop over terms

    // Compute AtA and add to normals.
    // AtA is symmetric, so we calculate the upper-triangular
    // portion
    // and leave the lower triangle empty.  But FORTRAN uses column
    // major ordering, so we'll tell the routine it is a lower-triangular
    // matrix.
    for (i = 0; i < iNumParms; ++i)
    {
      for (j = i; j < iNumParms; ++j)
      {
        ppadfN[i][j] += (adfAs[i] * adfAs[j] + adfAl[i] * adfAl[j]);
      }// j loop over columns in N
    }// i loop over rows in N

    // Compute misclosures
    dfWs = C;  // This corresponds to C * DenCoef[0], which is always 1.
    dfWl = R;
    for (i = 0; i < iNumParms; ++i)
    {
      dfWs += adfX[i] * adfAs[i];
      dfWl += adfX[i] * adfAl[i];
    }

    // Compute Atw and add to U
    for (i = 0; i < iNumParms; ++i)
    {
      adfU[i] += (adfAs[i] * dfWs + adfAl[i] * dfWl);
    }
  }// iObs loop over observations to populate normal equations.

  // Fill in the lower triangle of the normal equations
  for (i = 0; i < iNumParms; ++i)
  {
    for (j = 0; j < i; ++j)
    {
      ppadfN[i][j] = ppadfN[j][i];
    }// j loop over columns in N
  }// i loop over rows in N


/* -------------------------------------------------------------------------- */
/*      Now add the prior weights for the coefficients.                       */
/* -------------------------------------------------------------------------- */
  for (i = 0; i < iNumParms; ++i)
  {
    ppadfN[i][i] += adfPx[i];
    adfU[i] += adfPx[i] * adfX[i];
  }

  /* -------------------------------------------------------------------------- */
  /*      Factor the normal equations                                           */
  /* -------------------------------------------------------------------------- */
  memcpy(ppadfNfact[0], ppadfN[0], iMatNumEl * sizeof(double));
  iStatus = mat_inv(ppadfNfact, iNumParms, true);
  if (0 != iStatus)
  {
    throw pci::DataException(FormatString("Coefficient solver : Matrix invert failed at parameter %d", (int)iStatus),
      LOCATION);
  }

  /* -------------------------------------------------------------------------- */
  /*      Solve for corrections to parameters and apply them                    */
  /* -------------------------------------------------------------------------- */
  MatVecMult(ppadfNfact, adfU, iNumParms, iNumParms, adfDx);
  for (i = 0; i < iNumParms; ++i)
  {
    adfX[i] -= adfDx[i];
  }

  /* -------------------------------------------------------------------------- */
  /*      Now we iterate until the parameters converge.  We                     */
  /*      don't need to repopulate the normal equations because the             */
  /*      parameters do not appear in the partial derivatives...we              */
  /*      just recompute the misclosures and parameter updates.                 */
  /* -------------------------------------------------------------------------- */
      // Check our current convergence level (amount of correction to parameters)
  double dfConvergence = fabs(adfDx[VecIdxMaxAbs(adfDx, iNumParms)]);

  if (true == mbReportFlag)
  {
    mcADDINFO(FormatString("Iteration 0: dx_max = %e\n", dfConvergence));
  }

  for (iIter = 1; iIter < mlMaxIter && dfConvergence > mdfConvergeTol; ++iIter)
  {
    memset(adfU, 0, iNumParms * sizeof(double));

    /* ------------------------------------------------------------------- */
    /*      Add observations to U vector one observation at a time.        */
    /* ------------------------------------------------------------------- */
    for (iObs = 0; iObs < iNumObs; ++iObs)
    {
      // Compute normalized quantities.
      P = (oLat[iObs] - dfLatOff) / dfLatScale;
      L = (oLon[iObs] - dfLongOff) / dfLongScale;
      H = (oHgt[iObs] - dfHeightOff) / dfHeightScale;
      R = (oRow[iObs] - dfLineOff) / dfLineScale;
      C = (oCol[iObs] - dfSampOff) / dfSampScale;

      // Compute factors to go with the RPCs
      CalcLPHTerms(L, P, H, adfCoefMask, adfLPH);

      // Compute partial derivatives.
      for (i = 0; i < 20; ++i)
      {
        // Numerator coefficients
        adfAl[i] = -adfLPH[i];
        adfAs[i + 20] = -adfLPH[i];

        // Denominator coefficients
        if (0 < i)
        {
          adfAl[i + 39] = R * adfLPH[i];
          adfAs[i + 39] = C * adfLPH[i];
        }//
      }// i loop over terms

      // Compute misclosures
      dfWs = C;  // This corresponds to the first denominator coefficient, which is always 1.
      dfWl = R;
      for (i = 0; i < iNumParms; ++i)
      {
        dfWs += adfX[i] * adfAs[i];
        dfWl += adfX[i] * adfAl[i];
      }

      // Compute Atw and add to U
      for (i = 0; i < iNumParms; ++i)
      {
        adfU[i] += (adfAs[i] * dfWs + adfAl[i] * dfWl);
      }
    }// iObs loop over observations

/* ------------------------------------------------------------------- */
/*      Add prior constraints                                          */
/* ------------------------------------------------------------------- */
    for (i = 0; i < iNumParms; ++i)
    {
      adfU[i] += adfPx[i] * adfX[i];
    }

    /* ------------------------------------------------------------------- */
    /*      Update parameters                                              */
    /* ------------------------------------------------------------------- */
    MatVecMult(ppadfNfact, adfU, iNumParms, iNumParms, adfDx);
    for (i = 0; i < iNumParms; ++i)
    {
      adfX[i] -= adfDx[i];
    }

    dfConvergence = fabs(adfDx[VecIdxMaxAbs(adfDx, iNumParms)]);

    if (true == mbReportFlag)
    {
      mcADDINFO(FormatString("Iteration %d: dx_max = %e\n", iIter, dfConvergence));
    }
  }// iter loop over iterations

/* ------------------------------------------------------------------- */
/*      Throw an error if it didn't converge                           */
/* ------------------------------------------------------------------- */
  if (dfConvergence > mdfConvergeTol)
  {
    throw pci::DataException(FormatString("Error estimating rational function coefficients: convergence not achieve in %d iterations\nConvergence: %e\nTolerance: %e",
      mlMaxIter, dfConvergence, mdfConvergeTol),
      LOCATION);
  }

  if (true == mbReportFlag)
  {
    mcADDINFO(FormatString("Convergence achieved in %d iterations.\n\n", iIter));
  }

  /* ------------------------------------------------------------------- */
  /*      Populate the Rational Function object                          */
  /* ------------------------------------------------------------------- */
  double adfLineNumCoef[20];
  double adfLineDenCoef[20];
  double adfSampNumCoef[20];
  double adfSampDenCoef[20];
  for (i = 0; i < 20; ++i)
  {
    adfLineNumCoef[i] = adfX[i];
    adfSampNumCoef[i] = adfX[i + 20];
    if (0 < i)
    {
      adfLineDenCoef[i] = adfX[i + 39];
      adfSampDenCoef[i] = adfX[i + 39];
    }
    else
    {
      adfLineDenCoef[i] = 1.0;
      adfSampDenCoef[i] = 1.0;
    }
  }
  CreateRpcMathModel(padfOff, padfScale, adfLineNumCoef, adfLineDenCoef,
    adfSampNumCoef, adfSampDenCoef, poCoordSys, poModel);

  FreeMat(ppadfN);
  FreeMat(ppadfNfact);
  return 0;
}// LsqEstimateCommonDenom
