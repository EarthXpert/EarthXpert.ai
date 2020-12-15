/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/rationalmodel.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/exceptions/exexcept.h"
#include "core/math/mutils.h"
#include "core/utils/typehelper.h"
#include "sensormodel/rmutils.h"

#include <algorithm>
#include <cassert>

using namespace earthxpert;

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
                             SPSpatialRefSys groundSRS)
  : mNumCols(numCols), mNumRows(numRows),
    mNormalFactors(normalFators), mLineNumCoef(lineNumCoef),
    mLineDenCoef(lineDenCoef), mSampNumCoef(sampNumCoef),
    mSampDenCoef(sampDenCoef), mGroundSRS(groundSRS) {
  // get approximate image dimensions from normalization factors
  if (0 == mNumCols)
    mNumCols = static_cast<int>(nfc[eOSAMP] * 2);
  if (0 == mNumRows)
    mNumRows = static_cast<int>(nfc[eOLINE] * 2);

  CheckModel();

  mInvScale[eISLINE] = 1.0 / nfc[eSLINE];
  mInvScale[eISSAMP] = 1.0 / nfc[eSSAMP];
  mInvScale[eISLAT] = 1.0 / nfc[eSLAT];
  mInvScale[eISLON] = 1.0 / nfc[eSLON];
  mInvScale[eISHGT] = 1.0 / nfc[eSHGT];

  mTol_c = mInvScale[eISSAMP] * kRatio;
  mTol_r = mInvScale[eISLINE] * kRatio;

  EnableCubeS2G(false);
}

RationalModel::RationalModel(const std::array<double, NumFact> &normalFators,
                             const std::array<double, NumCoef> &lineNumCoef,
                             const std::array<double, NumCoef> &lineDenCoef,
                             const std::array<double, NumCoef> &sampNumCoef,
                             const std::array<double, NumCoef> &sampDenCoef,
                             SPSpatialRefSys groundSRS)
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

    if (!std::equal(std::cbegin(mNormalFactors), std::cend(mNormalFactors),
      std::cbegin(rhs.mNormalFactors), equalD))
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

    if (mCorrAlgo != rhs.mCorrAlgo)
      return false;
    if (!std::equal(std::cbegin(mLineCorr), std::cend(mLineCorr),
      std::cbegin(rhs.mLineCorr), equalD))
      return false;
    if (!std::equal(std::cbegin(mSampCorr), std::cend(mSampCorr),
      std::cbegin(rhs.mSampCorr), equalD))
      return false;
  }
  return true;
}

/**
 *  \brief Check coefficients if they represent a valid model.
 *  \except Throw invalid_arguments if it is a bad model.
 */
void RationalModel::CheckModel() const {
  if (nfc[eSLINE] <= 0.0 || nfc[eSSAMP] <= 0.0)
    throw std::invalid_argument(_("Line or Sample scale must be positive."));
  if (nfc[eSLINE] < 1.0 || nfc[eSSAMP] < 1.0)
    throw std::invalid_argument(_("Line or Sample scale is too small."));
  if (nfc[eSLAT] <= 0.0 || nfc[eSLON] <= 0.0)
    throw std::invalid_argument(_("Latitude or Longitude scale must be positive."));
  if (nfc[eSLAT] < kTol || nfc[eSLON] < kTol)
    throw std::invalid_argument(_("Latitude or Longitude scale is too small."));
  if (nfc[eSHGT] <= 0.0)
    throw std::invalid_argument(_("Height scale must be positive."));
  if (nfc[eSHGT] < 1.0)
    throw std::invalid_argument(_("Height scale is too small."));

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
  if (ModelType::eMT_RATIONAL != rhs.GetModelType())
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
  const auto sensor = ImageToSensor(image);

  Point3d uvw{0.0, 0.0, (ground.z - nfc[eOHGT]) * mInvScale[eISHGT]}; // lph
  if (!SensorToVirtual(sensor, uvw))
    return false;

  ground.x = nfc[eOLON] + uvw.x * nfc[eSLON];
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
 */
bool RationalModel::GroundToImage(const Point3d &ground, Point2d &image) const {
  Point2d sensor{ 0.0, 0.0 };  // normalized ground coordinates
  if (!GroundToSensor(ground, sensor))
    return false;

  const auto imageAct = SensorToImageActual(sensor);
  image = imageAct + 0.5;
  return true;
}

/**
 *  \brief Set 2d correction polynomials in image/sensor space.
 *
 *  \param [in] lineCorr 2nd-order polynomial for line correction
 *  \param [in] sampCorr 2nd-order polynomial for sample correction
 *  \param [in] corrAlgo correction algoirthm
 */
void RationalModel::SetCorrection(const std::array<double, NumCorr> &lineCorr,
  const std::array<double, NumCorr> &sampCorr, const CorrAlgo corrAlgo) {
  mLineCorr = lineCorr;
  mSampCorr = sampCorr;
  mCorrAlgo = corrAlgo;

  mCorrFlag = std::any_of(std::cbegin(mLineCorr), std::cend(mLineCorr), NotZero<>()) ||
    std::any_of(std::cbegin(mSampCorr), std::cend(mSampCorr), NotZero<>());
}

/**
 *  \brief Get 2d correction polynomials in image/sensor space.
 *
 *  \param [out] lineCorr 2nd-order polynomial for line correction
 *  \param [out] sampCorr 2nd-order polynomial for sample correction
 *  \param [in] corrAlgo correction algoirthm
 */
void RationalModel::GetCorrection(std::array<double, NumCorr> &lineCorr,
  std::array<double, NumCorr> &sampCorr, const CorrAlgo corrAlgo) const {
  if (mCorrAlgo == corrAlgo) {
    lineCorr = mLineCorr;
    sampCorr = mSampCorr;
    return;
  }

  if (eCA_IMAGE == mCorrAlgo) {
    assert(eCA_SENSOR == corrAlgo);
  } // convert eCA_IMAGE to eCA_SENSOR
  else {
    assert(eCA_IMAGE == corrAlgo);
  } // convert eCA_SENSOR to eCA_IMAGE
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
  mNormalFactors = rhs.mNormalFactors;
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
  mNormalFactors = rhs.mNormalFactors;
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
  mNormalFactors.fill(0.0);
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
  mCorrAlgo = eCA_IMAGE;

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

  if (mCorrFlag) {  // apply correction
    const EqualD equalD(0.001);  // [unit: pixel]
    Point2d imageP{ imageApp };

    for (int iter = 0; iter < kMaxIterIAA; ++iter) {
      const auto imageCorr = CalcCorrection(imageApp);
      imageApp = image - imageCorr;
      if (equalD(imageApp.x, imageP.x) && equalD(imageApp.y, imageP.y))
        break;

      imageP = imageApp;
    }
  }
  return imageApp;
}

Point2d RationalModel::SensorActualToApparant(const Point2d& sensor) const {
  Point2d sensorApp{ sensor };

  return sensorApp;
}

/**
 *  \brief calculate the correction in image/sensor space.
 *
 *  \param [in] cam The image/sensor position compatible with RPCs
 *  \return Returns {col, row} corrections
 */
Point2d RationalModel::CalcCorrection(const Point2d& cam) const {
  auto scc = mSampCorr.data();
  auto lcc = mLineCorr.data();
  const double CR[3] = { cam.x * cam.x, cam.x * cam.y, cam.y * cam.y };

  return { scc[eCO] + scc[eCS] * cam.x + scc[eCL] * cam.y +
    scc[eCS2] * CR[0] + scc[eCSL] * CR[1] + scc[eCL2] * CR[2],
           lcc[eCO] + lcc[eCS] * cam.x + lcc[eCL] * cam.y +
    lcc[eCS2] * CR[0] + lcc[eCSL] * CR[1] + lcc[eCL2] * CR[2] };
}

/**
 *  \brief Compute the combinations of normalized longitude, latitude,
 *  and height that go along with the rational function.
 *
 *  \param [in] u normalized longitude
 *  \param [in] v normalized latitude
 *  \param [in] w normalized height
 *  \param [out] uvw array of combined terms.
 */
void RationalModel::CalcTerms(const double u, const double v,
                              const double w, double *uvw) const {
  uvw[eC] = 1.0;
  uvw[eL] = u;
  uvw[eP] = v;
  uvw[eH] = w;
  uvw[eLP] = u * v;
  uvw[eLH] = u * w;
  uvw[ePH] = v * w;
  uvw[eLL] = u * u;
  uvw[ePP] = v * v;
  uvw[eHH] = w * w;
  uvw[ePLH] = uvw[eLP] * w; // L * P * H
  uvw[eLLL] = uvw[eLL] * u; // L * L * L
  uvw[eLPP] = u * uvw[ePP]; // L * P * P
  uvw[eLHH] = u * uvw[eHH]; // L * H * H
  uvw[eLLP] = uvw[eLL] * v; // L * L * P
  uvw[ePPP] = uvw[ePP] * v; // P * P * P
  uvw[ePHH] = v * uvw[eHH]; // P * H * H
  uvw[eLLH] = uvw[eLL] * w; // L * L * H
  uvw[ePPH] = uvw[ePP] * w; // P * P * H
  uvw[eHHH] = uvw[eHH] * w; // H * H * H
}

/**
 *  \brief Compute the polynomials using RPCs form the normalized longitude, latitude, height.
 *
 *  \param [in] u normalized longitude
 *  \param [in] v normalized latitude
 *  \param [in] w normalized height
 *  \return Returns line numerator, line denominator, sample numerator, sample denominator.
 */
Tuple4d RationalModel::CalcPolynomials(const double u, 
  const double v, const double w) const {
  double uvw[NumCoef];  // combinations of u, v, w for polynomial terms
  CalcTerms(u, v, w, uvw);

  double Ln{}, Ld{}; // Line numerator/denominator
  double Sn{}, Sd{}; // Sample numerator/denominator

  for (int i = 0; i < NumCoef; ++i) {
    Ln += lnc[i] * uvw[i];
    Ld += ldc[i] * uvw[i];
    Sn += snc[i] * uvw[i];
    Sd += sdc[i] * uvw[i];
  }

  if (fabs(Ld) < kTol || fabs(Sd) < kTol) 
    throw std::underflow_error(_("RationalModel: denominator is 0"));

  return { Ln, Ld, Sn, Sd };
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
 *    x/c  normalized column
 *    y/r  normalized row
 *    x'/c' estimated normalized column
 *    y'/r' estimated normalized row
 *    u' estimated normalized longitude
 *    v' estimated normalized latitude
 *    w  normalized height
 *
 *    x' = Sn(u', v', w) / Sd(u', v', w)   // approx. col/row
 *    y' = Ln(u', v', w) / Ld(u', v', w)
 *    dx = x' - x                          // misclosure
 *    dy = y' - y
 *    u = u' - du                          // update
 *    v = v' - dv
 *
 *    Formula comes from consideration of a Taylor series expansion
 *    about the initial approx. position (u', v')
 *
 *    | dx |  = | d(Sn/Sd)_du  d(Sn/Sd)_dv |  | du |
 *    | dy |  = | d(Ln/Ld)_du  d(Ln/Ld)_dv |  | dv |
 *
 *    [du dv] are obtained by inverting the design matrix and multiplying by
 * [dx, dy]. This iternative procedure is repeated until [dx, dy] are
 * sufficiently small.
 *
 *  \remark the scale factors eSSAMP, eSLINE are reduced for a single image.
 *  \ref RationalStereoModel::ImagesToGround()
 */
bool RationalModel::SensorToVirtual(const double x, const double y,
  const double w, double& u, double& v) const {
  if (mCubeS2G) {
    std::tie(u, v) = mCubeS2G->GetVal2(x, y, w);
  }
  else { // direct estimates for u, v
    if (!SensorToVirtualLinear(x, y, w, u, v))
      return false;
  }
  u = std::clamp(u, -1.2, 1.2);
  v = std::clamp(v, -1.2, 1.2);

  // refine the estimated ground horizontal position iteratively
  bool recalcFlag{ true }; // Flag indicating whether partial derivatives should be recomputed

  // Partial derivatives of Fc = (Sn/Sd) and Fr = (Ln/Ld) w.r.t. u and v
  // A[2][2] = | dFc_du  dFc_dv |
  //           | dFr_du  dFr_dv |
  double dFc_du{}, dFc_dv{}, dFr_du{}, dFr_dv{};
  double invDet{}; // Inverse of determinant of the partial derivative matrix

  int iter{};
  for (iter = 0; iter < kMaxIterS2G; ++iter) {
    const auto [Ln, Ld, Sn, Sd] = CalcPolynomials(u, v, w);

    const double invSd = 1.0 / Sd;
    const double invLd = 1.0 / Ld;
    const double xp = Sn * invSd;    // estimated normalized col/row
    const double yp = Ln * invLd;

    // recompute the partial derivatives of the sample & line w.r.t u, v
    if (recalcFlag) {
      // pre-compute partials TODO
      double duvw_du[NumCoef], duvw_dv[NumCoef];
      Duvw_duv(u, v, w, duvw_du, duvw_dv);

      const auto [dSn_du, dSn_dv] = D_duv(snc, duvw_du, duvw_dv); // Partial derivative of Sn w.r.t u, v
      const auto [dSd_du, dSd_dv] = D_duv(sdc, duvw_du, duvw_dv); // Partial derivative of Sd w.r.t u, v
      const auto [dLn_du, dLn_dv] = D_duv(lnc, duvw_du, duvw_dv); // Partial derivative of Ln w.r.t u, v
      const auto [dLd_du, dLd_dv] = D_duv(ldc, duvw_du, duvw_dv); // Partial derivative of Ld w.r.t u, v

      // Partial derivatives of (Sn/Sd) and (Ln/Ld) w.r.t u, v
      dFc_du = (dSn_du - xp * dSd_du) * invSd; // (Sd * dSn_du - Sn * dSd_du) / (Sd * Sd)
      dFc_dv = (dSn_dv - xp * dSd_dv) * invSd; // (Sd * dSn_dv - Sn * dSd_dv) / (Sd * Sd)
      dFr_du = (dLn_du - yp * dLd_du) * invLd; // (Ld * dLn_du - Ln * dLd_du) / (Ld * Ld)
      dFr_dv = (dLn_dv - yp * dLd_dv) * invLd; // (Ld * dLn_dv - Ln * dLd_dv) / (Ld * Ld)

      // determinant of the partial derivative matrix
      const double det = dFc_du * dFr_dv - dFc_dv * dFr_du;
      if (kTol >= fabs(det))
        return false;

      invDet = 1.0 / det;
    }

    // normalized col & row misclosures
    // b[2] = | dx  dy |
    const double dx = xp - x; // Normalized column misclosure
    const double dy = yp - y; // Normalized row misclosure

    // do not re-compute the partial derivatives within 1 pixel
    if (fabs(dx) < mInvScale[eISSAMP] && fabs(dy) < mInvScale[eISLINE]) {
      recalcFlag = false;
    }

    // delta of u and v by solving Ax = b
    const double du = (dFr_dv * dx - dFc_dv * dy) * invDet; // Normalized longitude error
    const double dv = (-dFr_du * dx + dFc_du * dy) * invDet; // Normalized latitude error

    u -= du;
    v -= dv;

    // check if still diverging after several iterations
    if (5 < iter && (kTol_d < fabs(du) || kTol_d < fabs(dv)))
      return false;

    // terminate if converged
    if (fabs(dx) < mTol_c && fabs(dy) < mTol_r)
      break;
  }

  // return false if reaching max. iterations
  return (kMaxIterS2G > iter);
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground position (analytic).
 *
 *  \param [out] image estimated normalized col/row
 *  \return Return {dFc_du, dFc_dv, dFr_du, dFr_dv}
 */
Tuple4d RationalModel::CalcJacobianA(const double w, const double u, 
  const double v, double& xp, double& yp) const {
  return {};
}

/**
 *  \brief Calculate Jacobian of rational functions at given ground position
 *  (central finite difference).
 *
 *  \param [out] image estimated normalized col/row
 *  \return Return {dFc_du, dFc_dv, dFr_du, dFr_dv}
 *
 *  \remark xx% faster than CalcJacobianA
 */
Tuple4d RationalModel::CalcJacobianNc(const double w, const double u, 
  const double v, double& xp, double& yp) const {
  return {};
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
 *    Fc(u, v) = Sn - Sd * x = 0
 *    Fr(u, v) = Ln - Ld * y = 0
 */
bool RationalModel::SensorToVirtualLinear(const double x, const double y,
  const double w, double &u, double &v) const {
  double A[2][2];  // Partial derivatives w.r.t. u and v
  A[0][0] = snc[eL] - sdc[eL] * x;
  A[0][1] = snc[eP] - sdc[eP] * x;
  A[1][0] = lnc[eL] - ldc[eL] * y;
  A[1][1] = lnc[eP] - ldc[eP] * y;

  // constant terms in RHS of equation Ax = b
  double b[2] = { (sdc[eC] + sdc[eH] * w) * x - snc[eC] - snc[eH] * w,
                  (ldc[eC] + ldc[eH] * w) * y - lnc[eC] - lnc[eH] * w };

  const double det = A[0][0] * A[1][1] - A[1][0] * A[0][1];
  if (kTol >= fabs(det)) {
    u = v = 0.0;
    return false;
  }

  const double inv = 1.0 / det;
  u = (A[1][1] * b[0] - A[0][1] * b[1]) * inv;
  v = (-A[1][0] * b[0] + A[0][0] * b[1]) * inv;

  return true;
}

/**
 *  Build a cube to provide fast X, Y coordinates used by SensorToVirtual.
 */
void RationalModel::BuildCubeS2G() {
  auto cubeS2G = std::make_unique<InterpCubeD>(2,
    -1.2, -1.2, -1.2, 1.2, 1.2, 1.2, 0.2, 0.2, 0.2);

  const auto numNodes = cubeS2G->GetNumNodes();
  std::vector<Point3d> cubeNodes(numNodes);
  cubeS2G->GetNodes(cubeNodes.data());

  auto cube = cubeS2G->GetArray();
  for (size_t i = 0; i < numNodes; ++i) {
    if (!SensorToVirtual(cubeNodes[i].x, cubeNodes[i].y,
      cubeNodes[i].z, cube[i*2], cube[i*2 + 1])) {
      throw std::exception(_("RationalModel: SensorToVirtual failed to project."));
    }
  }

  mCubeS2G = std::move(cubeS2G);
}
