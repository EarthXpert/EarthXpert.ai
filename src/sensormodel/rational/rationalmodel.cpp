/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalmodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/typehelper.h"
#include "sensormodel/rmutils.h"

#include <algorithm>
#include <cassert>

using namespace earthxpert;

static constexpr double kTol = std::numeric_limits<double>::epsilon();

/**
 *  \brief Construct a standard third-order rational function model.
 *
 *  \param [in] numCols Image width [pixel], specifying as 0 if unknown.
 *  \param [in] numRows Image height [pixel], specifying as 0 if unknown.
 *  \param [in] normalFators normalization factors including offsets & scales
 *  \param [in] lineNumCoef numerator polynomial of line formulae
 *  \param [in] lineDenCoef denominator polynomial of line formulae
 *  \param [in] sampNumCoef numerator polynomial of sample formulae
 *  \param [in] sampDenCoef denominator polynomial of sample formulae
 *  \param [in] groundCS ground coordinate system
 *  \remark The the image size is not stored in _rpc.txt/.rpb files.
 * If not known, good approx. values are estimated from offset & scale factors.
 */
RationalModel::RationalModel(const int numCols, const int numRows,
                             const std::array<double, NumFact> &normalFators,
                             const std::array<double, NumCoef> &lineNumCoef,
                             const std::array<double, NumCoef> &lineDenCoef,
                             const std::array<double, NumCoef> &sampNumCoef,
                             const std::array<double, NumCoef> &sampDenCoef,
                             std::shared_ptr<SpatialReferenceSystem> groundSRS)
  : mNumCols(numCols), mNumRows(numRows),
    mNormalFators(normalFators), mLineNumCoef(lineNumCoef),
    mLineDenCoef(lineDenCoef), mSampNumCoef(sampNumCoef),
    mSampDenCoef(sampDenCoef), mGroundSRS(groundSRS) {
  const auto nfc = mNormalFators.data();

  // get approximate image dimensions from normalization factors
  if (0 == mNumCols)
    mNumCols = static_cast<int>(nfc[eOSAMP] * 2);
  if (0 == mNumRows)
    mNumRows = static_cast<int>(nfc[eOLINE] * 2);

  CheckModel();

  mInvScale[eISLINE] = 1.0 / nfc[eSLINE];
  mInvScale[eISSAMP] = 1.0 / nfc[eSSAMP];
  mInvScale[eISLAT] = 1.0 / nfc[eSLAT];
  mInvScale[eISLONG] = 1.0 / nfc[eSLONG];
  mInvScale[eISHEIGHT] = 1.0 / nfc[eSHEIGHT];

  mTol_c = mInvScale[eISSAMP] * kRatio;
  mTol_r = mInvScale[eISLINE] * kRatio;

  EnableCubeS2G(false);
}

RationalModel::RationalModel(const std::array<double, NumFact> &normalFators,
                             const std::array<double, NumCoef> &lineNumCoef,
                             const std::array<double, NumCoef> &lineDenCoef,
                             const std::array<double, NumCoef> &sampNumCoef,
                             const std::array<double, NumCoef> &sampDenCoef,
                             std::shared_ptr<SpatialReferenceSystem> groundSRS)
  : RationalModel(0, 0, normalFators, lineNumCoef, lineDenCoef,
    sampNumCoef, sampDenCoef, groundSRS) {
}

/**
 *  Copy constructor
 *
 *  \param [in] rhs Object to copy from
 */
RationalModel::RationalModel(const RationalModel &rhs) { CopyFrom(rhs); }

/**
 *  Move constructor
 *
 *  \param [in] rhs Object to move from
 */
RationalModel::RationalModel(RationalModel &&rhs) noexcept {
  MoveFrom(std::move(rhs));
}

/**
 *  Assignment operator
 *
 *  \param [in] rhs Object to copy from
 *  \return A reference to the newly modified object
 */
RationalModel &RationalModel::operator=(const RationalModel &rhs) {
  if (this != &rhs) {
    CopyFrom(rhs);
  }
  return *this;
}

/**
 *  Move assignment operator
 *
 *  \param [in] rhs Object to move from
 *  \return A reference to the newly modified object
 */
RationalModel &RationalModel::operator=(RationalModel &&rhs) noexcept {
  if (this != &rhs) {
    MoveFrom(std::move(rhs));
  }
  return *this;
}

/**
 *  Equality operator
 *
 *  \param [in] rhs Object to compare with
 *  \return true if same object or equal, false otherwise.
 */
bool RationalModel::operator==(const RationalModel &rhs) const {
  if (this != &rhs) {
    const EqualD equalD;

    if (!std::equal(std::cbegin(mNormalFators), std::cend(mNormalFators),
      std::cbegin(rhs.mNormalFators), equalD))
      return false;
    if (!std::equal(std::cbegin(mLineNumCoef), std::cend(mLineNumCoef),
      std::cbegin(rhs.mLineNumCoef), equalD))
      return false;
    if (!std::equal(std::cbegin(mLineDenCoef), std::cend(mLineDenCoef),
      std::cbegin(rhs.mLineDenCoef), equalD))
      return false;
    if (!std::equal(std::cbegin(mSampNumCoef), std::cend(mSampNumCoef),
      std::cbegin(rhs.mSampNumCoef), equalD))
      return false;
    if (!std::equal(std::cbegin(mSampDenCoef), std::cend(mSampDenCoef),
      std::cbegin(rhs.mSampDenCoef), equalD))
      return false;

    if (*mGroundSRS != *(rhs.mGroundSRS))
      return false;
    if (!equalD(mErrBias, rhs.mErrBias) || !equalD(mErrRand, rhs.mErrRand))
      return false;

    if (!std::equal(std::cbegin(mLineCorr), std::cend(mLineCorr),
      std::cbegin(rhs.mLineCorr), equalD))
      return false;
    if (!std::equal(std::cbegin(mSampCorr), std::cend(mSampCorr),
      std::cbegin(rhs.mSampCorr), equalD))
      return false;

    if (mCorrAlgo != rhs.mCorrAlgo)
      return false;
  }
  return true;
}

/**
 *  Test inequality.
 *
 *  \param [in] rhs Object to compare with
 *  \return true if not equal.
 */
bool RationalModel::operator!=(const RationalModel &rhs) const {
  return !(operator==(rhs));
}

/**
 *  \brief Check coefficients if they represent a valid model.
 *  \except Throw invalid_arguments if it is a bad model.
 */
void RationalModel::CheckModel() const {
  const auto nfc = mNormalFators.data();

  if (nfc[eSLINE] < 0.0 || nfc[eSSAMP] < 0.0)
    throw std::invalid_argument("Line or Sample scale must not be negative.");
  if (nfc[eSLINE] < 1.0 || nfc[eSSAMP] < 1.0)
    throw std::invalid_argument("Line or Sample scale is too small.");
  if (nfc[eSLAT] < 0.0 || nfc[eSLONG] < 0.0)
    throw std::invalid_argument("Latitude or Longitude scale must not be negative.");
  if (nfc[eSLAT] < kTol || nfc[eSLONG] < kTol)
    throw std::invalid_argument("Latitude or Longitude scale is too small.");
  if (nfc[eSHEIGHT] < 0.0)
    throw std::invalid_argument("Height scale must not be negative.");
  if (nfc[eSHEIGHT] < 1.0)
    throw std::invalid_argument("Height scale is too small.");

  // numerical reliability, not singular, contineous
  // non-zero denominator

}

/**
 *  \brief Compare two models.
 *
 *  \param [in] rhs The other model to compare
 *  \return Return true if two models represent exactly same geometry
 */
bool RationalModel::EqualTo(const ISensorModel &rhs) const {
  if (ModelType::eRATIONAL != rhs.GetModelType())
    return false;

  auto rationalModel = dynamic_cast<const RationalModel *>(&rhs);
  assert(rationalModel);

  return operator==(*rationalModel);
}

/**
 *  Image-to-ground forward projection.
 *
 *  \brief Given a 2d position in image coordinate system and a height
 *  in ground vertical coordinate system, output the ground horizontal position.
 *
 *  \param [in] image 2d position (col, row) in image coordinate system
 *  \param [in/out] ground 3d ground position (Long, Lat, Height),
 *      where Height is known, in ground coordinate system
 *  \return Returns true if the operation succeeds
 */
bool RationalModel::ImageToGround(const Point2d &image, Point3d &ground) const {
  auto nfc = mNormalFators.data();
  Point2d imageAct{ image.x - 0.5, image.y - 0.5 };

  const auto imageApp = ImageActualToApparant(imageAct);
  const auto sensor = ImageToSensor(imageApp);

  Point3d uvw{0.0, 0.0, (ground.z - nfc[eOHEIGHT]) * mInvScale[eISHEIGHT]}; // lph
  if (!SensorToGround(sensor, uvw))
    return false;

  ground.x = nfc[eOLONG] + uvw.x * nfc[eSLONG];
  ground.y = nfc[eOLAT] + uvw.y * nfc[eSLAT];
  return true;
}

/**
 *  Ground-to-image backward projection.
 *
 *  \brief Given a 3d ground position with reference to the ground coordinate
 * system, output the 2d image position in the image coordinate system.
 *
 *  \param [in] ground 3d ground position (Long, Lat, Height)
 *  \param [out] image 2d image position (col, row)
 *  \return Returns true if operation succeeds
 *
 *  \details More details
 */
bool RationalModel::GroundToImage(const Point3d &ground, Point2d &image) const {
  auto nfc = mNormalFators.data();

  // normalized ground coordinates
  const Point3d uvw = GroundNormalize(ground);
  Point2d sensor{ 0.0, 0.0 };
  if (!GroundToSensor(uvw, sensor))
    return false;

  const auto imageApp = SensorToImage(sensor);
  const auto imageAct = ImageApparantToActual(imageApp);
  image = imageAct + 0.5;
  return true;
}

/**
 *  \brief Set 2d correction polynomials in sensor space.
 *
 *  \param [in] lineCorr 2nd-order polynomial for line correction
 *  \param [in] sampCorr 2nd-order polynomial for sample correction
 *  \param [in] corrAlgo correction algoirthm
 */
void RationalModel::SetSensorCorrection(const std::array<double, NumCorr> &lineCorr,
  const std::array<double, NumCorr> &sampCorr, const CorrAlgo corrAlgo) {
  mLineCorr = lineCorr;
  mSampCorr = sampCorr;
  mCorrAlgo = corrAlgo;

  auto NotZero = [](const double val) { return 0.0 != val; };
  mCorrectionFlag = std::any_of(std::cbegin(mLineCorr), std::cend(mLineCorr), NotZero) ||
    std::any_of(std::cbegin(mSampCorr), std::cend(mSampCorr), NotZero);
}

/**
 *  \brief Get 2d correction polynomials in sensor space.
 *
 *  \param [out] lineCorr 2nd-order polynomial for line correction
 *  \param [out] sampCorr 2nd-order polynomial for sample correction
 *  \param [in] corrAlgo correction algoirthm
 */
void RationalModel::GetSensorCorrection(std::array<double, NumCorr> &lineCorr,
  std::array<double, NumCorr> &sampCorr, const CorrAlgo corrAlgo) const {
  if (mCorrAlgo == corrAlgo) {
    lineCorr = mLineCorr;
    sampCorr = mSampCorr;
    return;
  }

  if (eSTANDARD == mCorrAlgo) {
    assert(eENHANCED == corrAlgo);
  } // convert eSTANDARD to eENHANCED
  else {
    assert(eSTANDARD == corrAlgo);
  } // convert eENHANCED to eSTANDARD
}

void RationalModel::EnableCubeS2G(const bool enable) {
  if (!enable) {
    mCubeS2G.reset();
    return;
  }

  if (!mCubeS2G) 
    BuildCubeS2G();
}

/**
 *  Copy values from another object
 *
 *  \param [in] rhs Object to copy from
 */
void RationalModel::CopyFrom(const RationalModel &rhs) {
  mNormalFators = rhs.mNormalFators;
  mLineNumCoef = rhs.mLineNumCoef;
  mLineDenCoef = rhs.mLineDenCoef;
  mSampNumCoef = rhs.mSampNumCoef;
  mSampDenCoef = rhs.mSampDenCoef;

  mGroundSRS = rhs.mGroundSRS;

  mNumCols = rhs.mNumCols;
  mNumRows = rhs.mNumRows;

  mErrBias = rhs.mErrBias;
  mErrRand = rhs.mErrRand;

  mLineCorr = rhs.mLineCorr;
  mSampCorr = rhs.mSampCorr;
  mCorrAlgo = rhs.mCorrAlgo;

  mTol_c = rhs.mTol_c;
  mTol_r = rhs.mTol_r;

  mInvScale = rhs.mInvScale;
  mCubeS2G = rhs.mCubeS2G->Clone();
}

/**
 *  Move values from another object
 *
 *  \param [in] rhs Object to move from
 */
void RationalModel::MoveFrom(RationalModel &&rhs) noexcept {
  mNormalFators = rhs.mNormalFators;
  mLineNumCoef = rhs.mLineNumCoef;
  mLineDenCoef = rhs.mLineDenCoef;
  mSampNumCoef = rhs.mSampNumCoef;
  mSampDenCoef = rhs.mSampDenCoef;

  mGroundSRS = std::move(rhs.mGroundSRS);

  mNumCols = rhs.mNumCols;
  mNumRows = rhs.mNumRows;

  mErrBias = rhs.mErrBias;
  mErrRand = rhs.mErrRand;

  mLineCorr = rhs.mLineCorr;
  mSampCorr = rhs.mSampCorr;
  mCorrAlgo = rhs.mCorrAlgo;

  mTol_c = rhs.mTol_c;
  mTol_r = rhs.mTol_r;

  mInvScale = rhs.mInvScale;
  mCubeS2G = std::move(rhs.mCubeS2G);

  rhs.Clear();
}

void RationalModel::Clear() noexcept {
  mNormalFators.fill(0.0);
  mLineNumCoef.fill(0.0);
  mLineDenCoef.fill(0.0);
  mSampNumCoef.fill(0.0);
  mSampDenCoef.fill(0.0);

  mNumCols = 0;
  mNumRows = 0;

  mErrBias = 0;
  mErrRand = 0;

  mLineCorr.fill(0.0);
  mSampCorr.fill(0.0);
  mCorrAlgo = eSTANDARD;

  mInvScale.fill(0.0);
}

/**
 *  \brief Given a 2d position in image coordinate system,
 *  output the corrected 2d position in image coordinate system.
 *
 *   The rational model uses the formulation
 *
 *   col = col_rpc + CS0 + CSS * col_rpc + CSL * row_rpc + ...
 *   row = row_rpc + CL0 + CLS * col_rpc + CLL * row_rpc + ...
 *
 *   The uncorrected col/row are
 *
 *   col_rpc = col - (CS0 + CSS * col_rpc + CSL * row_rpc + ...)
 *   row_rpc = row - (CL0 + CLS * col_rpc + CLL * row_rpc + ...)
 *
 *   Alternatively assuming that CL0, CS0 be primary and CLL, CSL, etc. are
 *   secondary, approx. col/row are
 *
 *   col_rpc = (col - CS0) - (CSS * (col - CS0) + CSL * (row - CL0) + ...)
 *   row_rpc = (row - CL0) - (CLS * (col - CS0) + CLL * (row - CL0) + ...)
 *
 *  \param [in] image 2d position (col, row) in imagery coordinate system
 *  \return Return 2d position (x, y) in virtual sensor coordinate system
 *  \todo comapre Newton-Raphson method
 */
Point2d RationalModel::ImageActualToApparant(const Point2d& image) const {
  Point2d imageApp{ image };

  if (mCorrectionFlag) {  // apply correction
    const EqualD equalD(0.001);  // [unit: pixel]
    Point2d imageP{ imageApp };

    for (int i = 0; i < kMaxIterIAA; ++i) {
      const auto imageCorr = CalcCorrection(imageApp);
      imageApp = image - imageCorr;
      if (equalD(imageApp.x, imageP.x) && equalD(imageApp.y, imageP.y))
        break;

      imageP = imageApp;
    }
  }
  return imageApp;
}

/**
 *  \brief Given a 2d rpc position in image coordinate system,
 *  output the actual 2d position.
 *
 *  \param [in] image 2d position (col, row) in image coordinate system
 *  \return Return 2d actual position (col, row) in imagery coordinate system
 */
Point2d RationalModel::ImageApparantToActual(const Point2d& image) const {
  Point2d imageAct{ image };
  if (mCorrectionFlag) {  // apply correction
    const auto imageCorr = CalcCorrection(image);
    imageAct += imageCorr;
  }
  return imageAct;
}

/**
 *  \brief calculate the correction in image space.
 *
 *  \param [in] image The {col, row} position compatible with RPCs
 *  \return Returns {col, row} corrections
 */
Point2d RationalModel::CalcCorrection(const Point2d& image) const {
  auto scc = mSampCorr.data();
  auto lcc = mLineCorr.data();
  const double CR[3] = { image.x * image.x, image.x * image.y, image.y * image.y };

  return { scc[eCO] + scc[eCS] * image.x + scc[eCL] * image.y +
    scc[eCS2] * CR[0] + scc[eCSL] * CR[1] + scc[eCL2] * CR[2],
           lcc[eCO] + lcc[eCS] * image.x + lcc[eCL] * image.y +
    lcc[eCS2] * CR[0] + lcc[eCSL] * CR[1] + lcc[eCL2] * CR[2] };
}

/**
 *  Sensor-to-ground projection in virtual coordinate systems.
 *
 *  \brief Given a normalized 2d position in image coordinate system and
 *  a normalized height in ground vertical coordinate system, output
 *  the ground horizontal position.
 *
 *  \param [in] x/c Normalized col in sensor coordinate system
 *  \param [in] y/r Normalized row in sensor coordinate system
 *  \param [in] w Normalized height in ground coordinate system
 *  \param [out] u Normalized longitude in ground coordinate system
 *  \param [out] v Normalized latitude in ground coordinate system
 *  \return Returns true if the operation succeeds
 *
 *  \details The Rational Function model provide coefficients for projection
 *    from ground space to image space. However, no coeffcients are given
 *    for projection from image space to ground space. Therefore, an iterative
 *    procedure is designed to estimate an approximate start L, P position,
 *    to project it into image space and evaluate the misclosure w.r.t. the
 *    measured known col and row coordinates, and to use the misclosures to
 *    compute corrections to the inital L, P estimates.
 *
 *    The iterations are performed in normalized coordinates.
 *
 *    c  normalized column
 *    r  normalized row
 *    c' estimated normalized column
 *    r' estimated normalized row
 *    u' estimated normalized longitude
 *    v' estimated normalized latitude
 *    w  normalized height
 *
 *    c' = Sn(u', v', w) / Sd(u', v', w)   // approx. col/row
 *    r' = Ln(u', v', w) / Ld(u', v', w)
 *    dc = c' - c                          // misclosure
 *    dr = r' - r
 *    u = u' - du                          // update
 *    v = v' - dv
 *
 *    Formula comes from consideration of a Taylor series expansion
 *    about the initial approx. position (u', v')
 *
 *    | dc |  = | d(Sn/Sd)_du  d(Sn/Sd)_dv |  | du |
 *    | dr |  = | d(Ln/Ld)_du  d(Ln/Ld)_dv |  | dv |
 *
 *    [du dv] are obtained by inverting the design matrix and multiplying by
 * [dc, dr]. This iternative procedure is repeated until [dc, dr] are
 * sufficiently small.
 */
bool RationalModel::SensorToGround(const double x, const double y,
  const double w, double& u, double& v) const {
  if (mCubeS2G) {
    std::tie(u, v) = mCubeS2G->GetVal2(x, y, w);
  }
  else { // direct estimates for u, v
    if (!SensorToGroundLinear(x, y, w, u, v))
      return false;
  }
  u = std::clamp(u, -1.2, 1.2);
  v = std::clamp(v, -1.2, 1.2);

  // use short name for convenience
  auto snc = mSampNumCoef.data(); // Sample numerator coefficients
  auto sdc = mSampDenCoef.data(); // Sample denominator coefficients
  auto lnc = mLineNumCoef.data(); // Line numerator coefficients
  auto ldc = mLineDenCoef.data(); // Line denominator coefficients

  // refine the estimated ground horizontal position iteratively
  bool recalcFlag{ true }; // Flag indicating whether partial derivatives should be recomputed

  // Partial derivatives of Fc = (Sn/Sd) and Fr = (Ln/Ld) w.r.t. u and v
  // A[2][2] = | dFc_du  dFc_dv |
  //           | dFr_du  dFr_dv |
  double dFc_du{}, dFc_dv{}, dFr_du{}, dFr_dv{};
  double invDet{}; // Inverse of determinant of the partial derivative matrix

  int iter{};
  for (iter = 0; iter < kMaxIterS2G; ++iter) {
    const auto[Ln, Ld, Sn, Sd] = CalcPolynomials(u, v, w);

    const auto invSd = 1.0 / Sd;
    const auto invLd = 1.0 / Ld;

    // estimated normalized col/row
    const auto xp = Sn * invSd;
    const auto yp = Ln * invLd;

    // recompute the partial derivatives of the sample & line w.r.t u, v
    if (recalcFlag) {
      // pre-compute partials TODO
      double duvw_du[NumCoef], duvw_dv[NumCoef];
      Duvw_du(u, v, w, duvw_du);
      Duvw_dv(u, v, w, duvw_dv);

      const double dSn_du = D_du(snc, duvw_du);      // Partial derivative of Sn w.r.t u
      const double dSd_du = D_du(sdc, duvw_du);      // Partial derivative of Sd w.r.t u
      const double dSn_dv = D_dv(snc, duvw_dv);      // Partial derivative of Sn w.r.t v
      const double dSd_dv = D_dv(sdc, duvw_dv);      // Partial derivative of Sd w.r.t v

      const double dLn_du = D_du(lnc, duvw_du);      // Partial derivative of Ln w.r.t u
      const double dLd_du = D_du(ldc, duvw_du);      // Partial derivative of Ld w.r.t u
      const double dLn_dv = D_dv(lnc, duvw_dv);      // Partial derivative of Ln w.r.t v
      const double dLd_dv = D_dv(ldc, duvw_dv);      // Partial derivative of Ld w.r.t v

      // Partial derivatives of (Sn/Sd) and (Ln/Ld) w.r.t u, v
      dFc_du = (dSn_du - xp * dSd_du) * invSd; // (Sd * dSn_du - Sn * dSd_du) / (Sd * Sd)
      dFc_dv = (dSn_dv - xp * dSd_dv) * invSd; // (Sd * dSn_dv - Sn * dSd_dv) / (Sd * Sd)
      dFr_du = (dLn_du - yp * dLd_du) * invLd; // (Ld * dLn_du - Ln * dLd_du) / (Ld * Ld)
      dFr_dv = (dLn_dv - yp * dLd_dv) * invLd; // (Ld * dLn_dv - Ln * dLd_dv) / (Ld * Ld)

      // determinant of the partial derivative matrix
      const double det = dFc_du * dFr_dv - dFc_dv * dFr_du;
      if (std::numeric_limits<double>::epsilon() >= fabs(det)) {
        return false;
      }

      invDet = 1.0 / det;
    }

    // normalized col & row misclosures
    // b[2] = | dc  dr |
    const double dc = xp - x; // Normalized column misclosure
    const double dr = yp - y; // Normalized row misclosure

    // do not re-compute the partial derivatives within 1 pixel
    if (fabs(dc) < mInvScale[eISSAMP] && fabs(dr) < mInvScale[eISLINE]) {
      recalcFlag = false;
    }

    // delta of u and v by solving Ax = b
    const double du = (dFr_dv * dc - dFc_dv * dr) * invDet; // Normalized longitude error
    const double dv = (-dFr_du * dc + dFc_du * dr) * invDet; // Normalized latitude error

    u -= du;
    v -= dv;

    // check if still diverging after several iterations
    if (5 < iter && (kTol_d < fabs(du) || kTol_d < fabs(dv)))
      return false;

    // terminate if converged
    if (fabs(dc) < mTol_c && fabs(dr) < mTol_r)
      break;
  }

  // return false if reaching max. iterations
  if (kMaxIterS2G == iter) {
    return false;
  }

  return true;
}

/**
 *  \brief Compute the combinations of normalized longitude, latitude,
 *  and height that go along with the rational function.
 *
 *  \param [in] L normalized longitude
 *  \param [in] P normalized latitude
 *  \param [in] H normalized height
 *  \param [out] uvw/LPH array of combined terms.
 *  \remark may use uvw instead of LPH in case of the lowercase l that looks like the number 1.
 */
void RationalModel::CalcTerms(const double L, const double P,
                              const double H, double *LPH) const {
  LPH[eC] = 1.0;
  LPH[eL] = L;
  LPH[eP] = P;
  LPH[eH] = H;
  LPH[eLP] = L * P;
  LPH[eLH] = L * H;
  LPH[ePH] = P * H;
  LPH[eLL] = L * L;
  LPH[ePP] = P * P;
  LPH[eHH] = H * H;
  LPH[ePLH] = LPH[eLP] * H; // L * P * H
  LPH[eLLL] = LPH[eLL] * L; // L * L * L
  LPH[eLPP] = L * LPH[ePP]; // L * P * P
  LPH[eLHH] = L * LPH[eHH]; // L * H * H
  LPH[eLLP] = LPH[eLL] * P; // L * L * P
  LPH[ePPP] = LPH[ePP] * P; // P * P * P
  LPH[ePHH] = P * LPH[eHH]; // P * H * H
  LPH[eLLH] = LPH[eLL] * H; // L * L * H
  LPH[ePPH] = LPH[ePP] * H; // P * P * H
  LPH[eHHH] = LPH[eHH] * H; // H * H * H
}

/**
 *  \brief Compute the polynomials using RPCs form the normalized longitude, latitude, height.
 *
 *  \param [in] L normalized longitude
 *  \param [in] P normalized latitude
 *  \param [in] H normalized height
 *  \return Returns line numerator, line denominator, sample numerator, sample denominator.
 */
Tuple4d RationalModel::CalcPolynomials(const double L, 
  const double P, const double H) const {
  auto lnc = mLineNumCoef.data();
  auto ldc = mLineDenCoef.data();
  auto snc = mSampNumCoef.data();
  auto sdc = mSampDenCoef.data();

  double LPH[NumCoef];  // combinations of L, P, H for polynomial terms
  CalcTerms(L, P, H, LPH);

  double Ln{}, Ld{}; // Line numerator/denominator
  double Sn{}, Sd{}; // Sample numerator/denominator

  for (int i = 0; i < NumCoef; ++i) {
    Ln += lnc[i] * LPH[i];
    Ld += ldc[i] * LPH[i];
    Sn += snc[i] * LPH[i];
    Sd += sdc[i] * LPH[i];
  }

  if (fabs(Ld) < kTol || fabs(Sd) < kTol) 
    throw std::underflow_error("RationalModel: denominator is 0 in rational model");

  return { Ln, Ld, Sn, Sd };
}

/**
 *  \brief Do sensor-to-ground projection using linear coefficients.
 *
 *  \param [in] x/c normalized imagery col position
 *  \param [in] y/r normalized imagery row position
 *  \param [in] w normalized height
 *  \param [out] u normalized longitude
 *  \param [out] v normalized latitude
 *
 *  \details The Direct Linear Transform is a special case of the
 *    rational model where only linear terms are used.
 *
 *        snc0 + snc1 * u + snc2 * v + snc3 * w    Sn
 *    x = ------------------------------------- = ----
 *        sdc0 + sdc1 * u + sdc2 * v + sdc3 * w    Sd
 *
 *        lnc0 + lnc1 * u + lnc2 * v + lnc3 * w    Ln
 *    y = ------------------------------------- = ----
 *        ldc0 + ldc1 * u + ldc2 * v + ldc3 * w    Ld
 *
 *    Given observed values for x, y, and w, we have two equations
 *    in two unknowns (u and v) for solving directly for
 *    the normalized virtual ground coordinates.
 *
 *    The two equations are:
 *
 *    Fc(u, v) = Sn - Sd * c = 0
 *    Fr(u, v) = Ln - Ld * r = 0
 */
bool RationalModel::SensorToGroundLinear(const double x, const double y,
  const double w, double &u, double &v) const {
  // use short name for convenience
  auto snc = mSampNumCoef.data();
  auto sdc = mSampDenCoef.data();
  auto lnc = mLineNumCoef.data();
  auto ldc = mLineDenCoef.data();

  double A[2][2];  // Partial derivatives w.r.t. u and v
  A[0][0] = snc[eL] - sdc[eL] * x;
  A[0][1] = snc[eP] - sdc[eP] * x;
  A[1][0] = lnc[eL] - ldc[eL] * y;
  A[1][1] = lnc[eP] - ldc[eP] * y;

  // constant terms in RHS of equation Ax = b
  double b[2] = { (sdc[eC] + sdc[eH] * w) * x - snc[eC] - snc[eH] * w,
                  (ldc[eC] + ldc[eH] * w) * y - lnc[eC] - lnc[eH] * w };

  const double det = A[0][0] * A[1][1] - A[1][0] * A[0][1];
  if (std::numeric_limits<double>::epsilon() >= fabs(det)) {
    u = 0.0;
    v = 0.0;
    return false;
  }

  const double inv = 1.0 / det;
  u = (A[1][1] * b[0] - A[0][1] * b[1]) * inv;
  v = (-A[1][0] * b[0] + A[0][0] * b[1]) * inv;

  return true;
}

/**
 *  Build a cube to provide fast X, Y coordinates used by SensorToGround.
 */
void RationalModel::BuildCubeS2G() {
  auto cubeS2G = std::make_unique<InterpCubeD>(2,
    -1.2, -1.2, -1.2, 1.2, 1.2, 1.2, 0.2, 0.2, 0.2);

  const auto numNodes = cubeS2G->GetNumNodes();
  std::vector<Point3d> cubeNodes(numNodes);
  cubeS2G->GetNodes(cubeNodes.data());

  auto cube = cubeS2G->GetArray();
  for (size_t i = 0; i < numNodes; ++i)
  {
    if (!SensorToGround(cubeNodes[i].x, cubeNodes[i].y, cubeNodes[i].z,
                        cube[i*2], cube[i*2 + 1])) {
      throw std::exception(_("RationalModel: SensorToGround failed to project."));
    }
  }

  mCubeS2G = std::move(cubeS2G);
}
