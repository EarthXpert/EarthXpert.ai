/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/math/interpcube.h"
#include "sensormodel/sensormodel.h"

#include <array>

namespace earthxpert {
/**
 *  The class RationalModel implements the replacement geometry
 *  model for various physical sensors.
 *
 *  \detail The rational function model is defined by formulae
 *
 *       Ln(L, P, H)
 *  r = ------------
 *       Ld(L, P, H)
 *
 *      lnc0 + lnc1 * L + lnc2 * P + lnc3 * H + ... + lnc19 * H * H * H
 *    = ---------------------------------------------------------------
 *      ldc0 + ldc1 * L + ldc2 * P + ldc3 * H + ... + ldc19 * H * H * H
 *
 *       Sn(L, P, H)
 *  c = ------------
 *       Sd(L, P, H)
 *
 *      snc0 + snc1 * L + snc2 * P + snc3 * H + ... + snc19 * H * H * H
 *    = ---------------------------------------------------------------
 *      sdc0 + sdc1 * L + sdc2 * P + sdc3 * H + ... + sdc19 * H * H * H
 *
 *  where
 *
 *      r the normalized line position
 *      c the normalized sample position
 *      L the normalized longitude position
 *      P the normalized latitude position
 *      H the normalized height
 *      lnc 20 line numerator coefficients given in lineNumCoef
 *      ldc 20 line denominator coefficients given in lineDenCoef
 *      snc 20 sample numerator coefficients given in sampNumCoef
 *      sdc 20 sample denominator coefficients given in sampDenCoef
 */
class EXSENSORMODEL_EXPORTS RationalModel final : public SensorModel {
public:
  // index of normalization factors
  enum NormalizationFactor {
    eOLINE = 0,   // line offset
    eOSAMP = 1,   // sample offset
    eOLAT = 2,    // latitude offset
    eOLONG = 3,   // longitude offset
    eOHEIGHT = 4, // height offset
    eSLINE = 5,   // line scale
    eSSAMP = 6,   // sample scale
    eSLAT = 7,    // latitude scale
    eSLONG = 8,   // longitude scale
    eSHEIGHT = 9, // height scale
    NumFact = 10  // number of factors
  };
  using nf = NormalizationFactor;

  // index of inversed scale factors
  enum InvScaleFactor {
    eISLINE = 0,   // line inversed scale
    eISSAMP = 1,   // sample inversed scale
    eISLAT = 2,    // latitude inversed scale
    eISLONG = 3,   // longitude inversed scale
    eISHEIGHT = 4, // height inversed scale
    NumInvScl = 5  // number of inversed scales
  };
  using isf = InvScaleFactor;

  // index of 3rd-order polynomial coefficients
  // The order of terms is from NITF RPC00B standard (STDI-0002).
  enum RationalPolyCoeff {
    eC = 0,      // constant term
    eL = 1,      // L normalized longitude
    eP = 2,      // P normalized latitude
    eH = 3,      // H normalized height
    eLP = 4,     // L * P
    eLH = 5,     // L * H
    ePH = 6,     // P * H
    eLL = 7,     // L * L
    ePP = 8,     // P * P
    eHH = 9,     // H * H
    ePLH = 10,   // P * L * H
    eLLL = 11,   // L * L * L
    eLPP = 12,   // L * P * P
    eLHH = 13,   // L * H * H
    eLLP = 14,   // L * L * P
    ePPP = 15,   // P * P * P
    ePHH = 16,   // P * H * H
    eLLH = 17,   // L * L * H
    ePPH = 18,   // P * P * H
    eHHH = 19,   // H * H * H
    NumCoef = 20 // number of coefficients of 3rd-order polynomial
  };
  using rpc = RationalPolyCoeff;

  // index of 2nd-order corrrection polynomial coefficients
  enum CorrCoeff {
    eCO = 0,    // correction offset
    eCS = 1,    // sample
    eCL = 2,    // line
    eCS2 = 3,   // sample squared
    eCSL = 4,   // sample * line
    eCL2 = 5,   // line squared
    NumCorr = 6 // number of correction coefficients of 2nd-order polynomial
  };

  // sensor correction algorithm
  // my experiments show that eENHANCED is better than eSTANDARD in accuracy at
  // an amount of 10~15%
  enum CorrAlgo {
    eSTANDARD, // standard correction with full pixel coordinates
    eENHANCED  // enhanced correction with normalized pixel coordinates
  };

  /**
   *  \brief Construct a standard third-order rational function model.
   *
   *  \param [in] numCols Image width [pixel], specifying as 0 if unknown.
   *  \param [in] numRows Image height [pixel], specifying as 0 if unknown.
   *  \param [in] normalization factors including offsets & scales
   *  \param [in] numerator polynomial of line formulae
   *  \param [in] denominator polynomial of line formulae
   *  \param [in] numerator polynomial of sample formulae
   *  \param [in] denominator polynomial of sample formulae
   *  \param [in] groundSRS ground coordinate system
   */
  RationalModel(int numCols, int numRows,
    const std::array<double, NumFact> &normalFators,
    const std::array<double, NumCoef> &lineNumCoef,
    const std::array<double, NumCoef> &lineDenCoef,
    const std::array<double, NumCoef> &sampNumCoef,
    const std::array<double, NumCoef> &sampDenCoef,
    std::shared_ptr<SpatialReferenceSystem> groundSRS);

  RationalModel(const std::array<double, NumFact> &normalFators,
    const std::array<double, NumCoef> &lineNumCoef,
    const std::array<double, NumCoef> &lineDenCoef,
    const std::array<double, NumCoef> &sampNumCoef,
    const std::array<double, NumCoef> &sampDenCoef,
    std::shared_ptr<SpatialReferenceSystem> groundSRS);

  RationalModel(const RationalModel &rhs);
  RationalModel(RationalModel &&rhs) noexcept;
  ~RationalModel() = default;

  RationalModel &operator=(const RationalModel &rhs);
  RationalModel &operator=(RationalModel &&rhs) noexcept;

  bool operator==(const RationalModel &rhs) const;
  bool operator!=(const RationalModel &rhs) const;

  std::unique_ptr<ISensorModel> Clone() const override {
    return std::make_unique<RationalModel>(*this);
  }

  /**
   *  Image-to-ground forward projection.
   *
   *  \brief Given a 2d position in image coordinate system and a height
   *  in the ground vertical coordinate system, output the ground horizontal
   *  position with respect to the ground coordinate system.
   *
   *  \param [in] image 2d position (col, row) in image coordinate system
   *  \param [in/out] ground 3d ground position (Long, Lat, Height) in ground
   * coordinate system \return Returns true if the operation succeeds
   *
   *  \details More details
   */
  bool ImageToGround(const Point2d &image, Point3d &ground) const override;

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
  bool GroundToImage(const Point3d &ground, Point2d &image) const override;

  // access
  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eRATIONAL;
  }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return pointer to a coordinate system
   */
  std::shared_ptr<SpatialReferenceSystem> GetCoordinateSystem() const
      noexcept override {
    return mGroundSRS;
  }

  /**
   *  \return Returns type of SRS
   *  \todo parse from mGroundSRS;
   */
  virtual SrsType GetSrsType() const noexcept { return eGEO; }

  void SetImageSize(const int numCols, const int numRows) noexcept {
    mNumCols = numCols;
    mNumRows = numRows;
  }

  /**
   *  \return Returns image width & height [pixel]
   */
  std::pair<int, int> GetImageSize() const noexcept override {
    return { mNumCols, mNumRows };
  }

  /**
   *  \return Returns the middle height
   */
  double GetMeanHeight() const noexcept { return mNormalFators[eOHEIGHT]; }

  /**
   *  \brief Get normalization factors including offsets & scales.
   */
  auto GetNormalFators() const noexcept { return mNormalFators.data(); }

  /**
   *  \brief Set bias and random errors in sensor space.
   *
   *  \param [in] errBias bias error of rational model
   *  \param [in] errRand random error of rational model
   */
  void SetErrors(const double errBias, const double errRand) noexcept {
    mErrBias = errBias;
    mErrRand = errRand;
  }

  /**
   *  \brief Get bias and random errors in sensor space.
   *
   *  \return Returns bias & random error2 of rational model
   */
  std::pair<double, double> GetErrors() const noexcept {
    return { mErrBias, mErrRand };
  }

  // block adjustment support
  /**
   *  \brief Set 2d correction polynomials in sensor space.
   *
   *  \param [in] lineCorr 2nd-order polynomial for line correction
   *  \param [in] sampCorr 2nd-order polynomial for sample correction
   *  \param [in] corrAlgo correction algoirthm
   */
  void SetSensorCorrection(const std::array<double, NumCorr> &lineCorr,
                           const std::array<double, NumCorr> &sampCorr,
                           CorrAlgo corrAlgo = eSTANDARD);

  /**
   *  \brief Get 2d correction polynomials in sensor space.
   *
   *  \param [out] lineCorr 2nd-order polynomial for line correction
   *  \param [out] sampCorr 2nd-order polynomial for sample correction
   *  \param [in] corrAlgo correction algoirthm
   */
  void GetSensorCorrection(std::array<double, NumCorr> &lineCorr,
                           std::array<double, NumCorr> &sampCorr,
                           CorrAlgo corrAlgo = eSTANDARD) const;

private:
  static constexpr int kMaxIterS2G = 10; // max. number of iterations allowed for SensorToGround
  static constexpr double kTolPartialS2G = 0.05; // tolerance
  static constexpr double kTol_d = 0.1; // tolerance proportional to ground range

  // rational polynomial coefficients
  std::array<double, NumFact>
      mNormalFators{}; // normalization factors including offsets & scales
  std::array<double, NumCoef>
      mLineNumCoef{}; // numerator polynomial of line formulae
  std::array<double, NumCoef>
      mLineDenCoef{}; // denominator polynomial of line formulae
  std::array<double, NumCoef>
      mSampNumCoef{}; // numerator polynomial of sample formulae
  std::array<double, NumCoef>
      mSampDenCoef{}; // denominator polynomial of sample formulae

  std::shared_ptr<SpatialReferenceSystem>
      mGroundSRS{}; // Ground coordinate system

  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product

  double mErrBias{}; // bias error [pixels]
  double mErrRand{}; // random error [pixels]

  void CheckModel() const;

  // corrections in sensor space
  std::array<double, NumCorr> mLineCorr{}; // polynomial for line correction
  std::array<double, NumCorr> mSampCorr{}; // polynomial for sample correction
  CorrAlgo mCorrAlgo{eSTANDARD};           // sensor correction method

  bool EqualTo(const ISensorModel &rhs) const override;
  void CopyFrom(const RationalModel &rhs);
  void MoveFrom(RationalModel &&rhs) noexcept;
  void Clear() noexcept;

  Point2d ImageActualToApparant(const Point2d& image) const;
  Point2d ImageApparantToActual(const Point2d& image) const;

  /**
   *  \brief Given a 2d position in image coordinate system,
   *  output the 2d position in virtual sensor coordinate system.
   */
  inline Point2d ImageToSensor(const Point2d& image) const {
    auto nfc = mNormalFators.data();
    return { (image.x - nfc[eOSAMP]) * mInvScale[eISSAMP],
             (image.y - nfc[eOLINE]) * mInvScale[eISLINE] };
  }

  /**
   *  \brief Given a 2d position in virtual sensor coordinate system,
   *  output the 2d rpc position in image coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in virtual sensor coordinate system
   *  \return Return 2d apparant rpc position (col, row) in imagery coordinate system
   */
  inline Point2d SensorToImage(const Point2d& sensor) const {
    auto nfc = mNormalFators.data();
    return { nfc[eOSAMP] + nfc[eSSAMP] * sensor.x,
             nfc[eOLINE] + nfc[eSLINE] * sensor.y };
  }

  Point2d CalcCorrection(const Point2d& image) const;

  /**
   *  Sensor-to-ground forward projection.
   *
   *  \brief Given a 2d position in virtual sensor coordinate system and a height
   *  in the ground vertical coordinate system, output the virtual ground horizontal
   *  position.
   *
   *  \param [in] sensor 2d position (x, y) in sensor coordinate system
   *  \param [in/out] uvw 3d ground position (u, v, w) or (L, P, H)
   * in virtual ground coordinate system
   *  \return Returns true if the operation succeeds
   */
  bool SensorToGround(const Point2d &sensor, Point3d &uvw) const {
    return SensorToGround(sensor.x, sensor.y, uvw.z, uvw.x, uvw.y);
  }

  /**
   *  Ground-to-sensor backward projection.
   *
   *  \brief Given a 3d ground position in the virtual ground coordinate
   * system, output the 2d position in the virtual sensor coordinate system.
   *
   *  \param [in] uvw 3d ground position (L, P, H)
   *  \param [out] sensor 2d sensor position (x, y)
   *  \return Returns true if operation succeeds
   */
  inline bool GroundToSensor(const Point3d& uvw,
    Point2d& sensor) const {
    const auto [Ln, Ld, Sn, Sd] = CalcPolynomials(uvw);

    // col/row computed from RFM
    sensor.x = Sn / Sd;
    sensor.y = Ln / Ld;
    return true;
  }

  void CalcTerms(const Point3d &uvw, double *const LPH) const {
      CalcTerms(uvw.x, uvw.y, uvw.z, LPH);
  }
  void CalcTerms(double L, double P, double H, double *const LPH) const;

  Tuple4d CalcPolynomials(const Point3d &uvw) const {
    return CalcPolynomials(uvw.x, uvw.y, uvw.z);
  }
  Tuple4d CalcPolynomials(double L, double P, double H) const;

  bool SensorToGround(const double x, const double y,
    const double w, double& u, double& v) const;

  bool SensorToGroundLinear(const Point2d &sensor, Point3d &uvw) const {
    return SensorToGroundLinear(sensor.x, sensor.y, uvw.z, uvw.x, uvw.y);
  }
  bool SensorToGroundLinear(double x, double y, double w, double &u,
                            double &v) const;

  // cached calculation for better performance
  double mTol_c{ 1.0e-6 }; // tolerance proportional to image range
  double mTol_r{ 1.0e-6 };
    
  std::array<double, NumInvScl> mInvScale{1.0}; // reciprocal of scale factors
  std::unique_ptr<InterpCubeD>
      mCubeS2G; // interpolation cube to provide approximate X, Y coordinates
                  // for ImageToGround()
  void BuildCubeS2G();

  bool mHasCorrection{false};
};
} // namespace earthxpert