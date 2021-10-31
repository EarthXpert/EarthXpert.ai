/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/math/interpcube.h"
#include "sensormodel/datatypes.h"
#include "sensormodel/sensormodel.h"

#include <array>

namespace earthxpert {
/**
 *  The class RationalModel implements the replacement geometry
 *  model for various physical sensors.
 *
 *  \detail The rational function model is defined by formulae
 *
 *       Ln(L, P, H)    p1(u, v, w)
 *  r = ------------ = -------------
 *       Ld(L, P, H)    p2(u, v, w)
 *
 *      lnc0 + lnc1 * L + lnc2 * P + lnc3 * H + ... + lnc19 * H * H * H
 *    = ---------------------------------------------------------------
 *      ldc0 + ldc1 * L + ldc2 * P + ldc3 * H + ... + ldc19 * H * H * H
 *
 *       Sn(L, P, H)    p3(u, v, w)
 *  c = ------------ = -------------
 *       Sd(L, P, H)    p4(u, v, w)
 *
 *      snc0 + snc1 * L + snc2 * P + snc3 * H + ... + snc19 * H * H * H
 *    = ---------------------------------------------------------------
 *      sdc0 + sdc1 * L + sdc2 * P + sdc3 * H + ... + sdc19 * H * H * H
 *
 *  where
 *
 *      r/y the normalized line position
 *      c/x the normalized sample position
 *      L/u the normalized longitude position
 *      P/v the normalized latitude position
 *      H/w the normalized height
 *      lnc 20 line numerator coefficients given in lineNumCoef
 *      ldc 20 line denominator coefficients given in lineDenCoef
 *      snc 20 sample numerator coefficients given in sampNumCoef
 *      sdc 20 sample denominator coefficients given in sampDenCoef
 *
 *  The formula with correction polynomials becomes
 *
 *  r' = r + lcc[eCO] + lcc[eCS] * S + lcc[eCL] * L +
 *       lcc[eCS2] * S^2 + lcc[eCSL] * S * L + lcc[eCL2] * L^2
 *
 *  c' = c + scc[eCO] + scc[eCS] * S + scc[eCL] * L +
 *       scc[eCS2] * S^2 + scc[eCSL] * S * L + scc[eCL2] * L^2
 *
 *  where
 *
 *      r' the corrected normalized line position
 *      c' the corrected normalized sample position
 *      L  the raw line position
 *      S  the raw sample position
 *      lcc 6 line correction coefficients given lineCorr
 *      scc 6 sample correction coefficients given sampCorr
 */
class EXSENSORMODEL_EXPORTS RationalModel /*final*/ : public SensorModel {
public:
  using nfi = NormalFactorIndex;  // index of normalization factors
  using nfa = NormalFactorArray;

  enum {
    eModelType = ModelType::eMT_RATIONAL
  };
  
  // index of 3rd-order polynomial coefficients
  // The order of terms is from NITF RPC00B standard (STDI-0002).
  enum PolyCoeff3rd {
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

  using PolyCoefArray = std::array<double, NumCoef>;

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
  // an amount of 10~15%, also ensuring higher-order fits better than lower-order
  enum CorrAlgo {
    eCA_IMAGE, // standard correction with full coordinates in image space [pixel]
    eCA_SENSOR // enhanced correction with normalized coordinates in sensor space
  };

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
   *  \param [in] groundSRS ground coordinate system
   */
  RationalModel(int numCols, int numRows, const NormalFactorArray &normalFators,
    const PolyCoefArray &lineNumCoef, const PolyCoefArray &lineDenCoef,
    const PolyCoefArray &sampNumCoef, const PolyCoefArray &sampDenCoef,
    std::shared_ptr<OGRSpatialRef> groundSRS);

  RationalModel(const NormalFactorArray &normalFators,
    const PolyCoefArray &lineNumCoef, const PolyCoefArray &lineDenCoef,
    const PolyCoefArray &sampNumCoef, const PolyCoefArray &sampDenCoef,
    std::shared_ptr<OGRSpatialRef> groundSRS);

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
   */
  bool ImageToGround(const Point2d& image, Point3d& ground) const override;
  bool ImageToGroundNR(const Point2d& image, Point3d& ground) const;

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
  bool GroundToImage(const Point3d &ground, Point2d &image) const override;

  // access
  /**
   * @return type of sensor model
   */
  ModelType GetModelType() const noexcept override {
    return ModelType::eMT_RATIONAL;
  }

  /**
   *  \brief Fetch the ground coordinate system where the sensor model operates.
   *  Use this SRS to construct the coordinate transformations
   *  from results of ImageToGround() or inputs to GroundToImage() methods.
   *
   *  \return pointer to a coordinate system
   */
  std::shared_ptr<OGRSpatialRef> GetCoordinateSystem() const
      noexcept override { return mGroundSRS; }

  /**
   *  \return Returns type of SRS
   *  \todo parse from mGroundSRS;
   */
  virtual SrsType GetSrsType() const noexcept { return eST_GEO; }

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
  float GetMeanHeight() const noexcept override { 
    return float(mNormalFactors[eOHGT]); 
  }

  std::pair<float, float> GetHeightRange() const noexcept override {
    return { float(mNormalFactors[eOHGT] - mNormalFactors[eSHGT]),
      float(mNormalFactors[eOHGT] + mNormalFactors[eSHGT]) };
  }

  /**
   *  \brief Get normalization factors including offsets & scales.
   */
  auto GetNormalFactors() const noexcept { return mNormalFactors.data(); }

  Point3d GetGroundCenter() const noexcept { 
    return { nfc[eOLON], nfc[eOLAT], nfc[eOHGT] };
  }

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
   *  \brief Set 2d correction polynomials in image/sensor space.
   *
   *  \param [in] lineCorr 2nd-order polynomial for line correction
   *  \param [in] sampCorr 2nd-order polynomial for sample correction
   *  \param [in] corrAlgo correction algoirthm
   */
  void SetCorrection(const std::array<double, NumCorr>& lineCorr,
    const std::array<double, NumCorr>& sampCorr,
    CorrAlgo corrAlgo = eCA_IMAGE);
  void SetCorrection(const double* lineCorr, const double* sampCorr,
    int order = 2, CorrAlgo corrAlgo = eCA_IMAGE);

  /**
   *  \brief Get 2d correction polynomials in image/sensor space.
   *
   *  \param [out] lineCorr 2nd-order polynomial for line correction
   *  \param [out] sampCorr 2nd-order polynomial for sample correction
   *  \param [in] corrAlgo correction algoirthm
   */
  void GetCorrection(std::array<double, NumCorr> &lineCorr,
                           std::array<double, NumCorr> &sampCorr,
                           CorrAlgo corrAlgo = eCA_IMAGE) const;

  void EnableCubeS2G(bool enable=true);

private:
  friend class RationalModelMarshaller;
  friend class RationalStereoModel;
  friend class RationalCeresSolver;

  static constexpr int kMaxIterS2G = 10; // max. number of iterations allowed for SensorToGround
  static constexpr int kMaxIterIAA = 5; // max. number of iterations allowed for ImageActualToApparant
  static constexpr double kRatio = 1.0e-4; // ratio of tolerance proportional to image range
  static constexpr double kLL = 1.0e-6; // step used to calculate numerical difference for lon/lat
  static constexpr double kTolLL = 1.0e-8; // 
  static constexpr double kFacLL = 0.5 / kLL;
  static constexpr double kTol = std::numeric_limits<double>::epsilon();
  static constexpr double kTol_px = 0.001; // tolerance in unit of pixel
  static constexpr double kTol_d = 0.1; // tolerance proportional to virtual ground range

  // rational polynomial coefficients
  NormalFactorArray mNormalFactors{}; // normalization factors including offsets & scales
  PolyCoefArray mLineNumCoef{}; // numerator polynomial of line formulae
  PolyCoefArray mLineDenCoef{}; // denominator polynomial of line formulae
  PolyCoefArray mSampNumCoef{}; // numerator polynomial of sample formulae
  PolyCoefArray mSampDenCoef{}; // denominator polynomial of sample formulae

  std::shared_ptr<OGRSpatialRef> mGroundSRS{}; // Ground coordinate system

  int mNumCols{}; // number of columns of the image product
  int mNumRows{}; // number of rows of the image product

  double mErrBias{}; // bias error [pixels]
  double mErrRand{}; // random error [pixels]

  void CheckModel() const;

  // corrections in sensor space
  std::array<double, NumCorr> mLineCorr{}; // polynomial for line correction
  std::array<double, NumCorr> mSampCorr{}; // polynomial for sample correction
  CorrAlgo mCorrAlgo{ eCA_IMAGE };           // sensor correction method

  // uncertainties for the enhanced algorithm [pixel]
  std::array<double, 3> mStdCorr{};   // constant, 1st-, 2nd-order coefficients

  bool EqualTo(const ISensorModel &rhs) const override;
  void CopyFrom(const RationalModel &rhs);
  void MoveFrom(RationalModel &&rhs) noexcept;
  void Clear() noexcept;

  Point2d ImageActualToApparant(const Point2d& image) const;
  Point2d SensorActualToApparant(const Point2d& sensor) const;

  /**
   *  \brief Given a 2d rpc position in image coordinate system,
   *  output the actual 2d position.
   *
   *  \param [in] image 2d position (col, row) in image coordinate system
   *  \return Return 2d actual position (col, row) in imagery coordinate system
   */
  Point2d ImageApparantToActual(const Point2d& image) const;

  /**
   *  \brief Given a 2d position in image coordinate system,
   *  output the 2d position in virtual sensor coordinate system.
   */
  inline Point2d ImageApparantToSensor(const Point2d& image) const;

  inline Point2d ImageToSensor(const Point2d& image) const;

  /**
   *  \brief Given a 2d position in virtual sensor coordinate system,
   *  output the 2d rpc position in image coordinate system.
   *
   *  \param [in] sensor 2d position (x, y) in virtual sensor coordinate system
   *  \return Return 2d apparant rpc position (col, row) in imagery coordinate system
   */
  inline Point2d SensorToImageApparant(const Point2d& sensor) const;

  inline Point2d SensorToImageActual(const Point2d& sensor) const;

  bool mCorrFlag{ false };   // flag indicating the correction status
  Point2d CalcCorrection(const Point2d& cam) const;

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
  bool SensorToVirtual(const Point2d &sensor, Point3d &uvw) const {
    return SensorToVirtual(sensor.x, sensor.y, uvw.z, uvw.x, uvw.y);
  }

  inline Point3d GroundToVirtual(const Point3d& ground) const;

  /**
   *  Ground-to-sensor backward projection.
   *
   *  \brief Given a 3d ground position in the virtual ground coordinate
   * system, output the 2d position in the virtual sensor coordinate system.
   *
   *  \param [in] uvw 3d normalized ground position (L, P, H)
   *  \param [out] sensor 2d sensor position (x, y)
   *  \return Returns true if operation succeeds
   */
  bool VirtualToSensor(const Point3d& uvw, Point2d& sensor) const;

  inline bool GroundToSensor(const Point3d& ground, Point2d& sensor) const {
    const Point3d uvw = GroundToVirtual(ground);
    return VirtualToSensor(uvw, sensor);
  }

  Tuple4d CalcPolynomials(const Point3d &uvw) const {
    return CalcPolynomials(uvw.x, uvw.y, uvw.z);
  }
  Tuple4d CalcPolynomials(double u, double v, double w) const;

  bool SensorToVirtual(double x, double y, double w, double& u, double& v) const;

  Tuple4d CalcJacobianA(double w, double u, double v, double& xp, double& yp) const;
  Tuple4d CalcJacobianNc(double w, double u, double v, double& xp, double& yp) const;

  bool SensorToVirtualLinear(double x, double y,
    double w, double &u, double &v) const;
  bool SensorToVirtualLinear(const Point2d &sensor, Point3d &uvw) const {
    return SensorToVirtualLinear(sensor.x, sensor.y, uvw.z, uvw.x, uvw.y);
  }
  
  // cached calculation for better performance
  double mTol_c{ 1.0e-7 }; // tolerance proportional to image range
  double mTol_r{ 1.0e-7 };
  
  // use short name for convenience
  const double* const nfc{ mNormalFactors.data() };
  const double* const snc{ mSampNumCoef.data() };
  const double* const sdc{ mSampDenCoef.data() };
  const double* const lnc{ mLineNumCoef.data() };
  const double* const ldc{ mLineDenCoef.data() };

  std::array<double, NumSclFact> mInvScale{1.0}; // reciprocal of scale factors
  
  // interpolation cube to provide approximate X, Y coordinates for ImageToGround()
  std::unique_ptr<InterpCubeD> mCubeS2G; 
  void BuildCubeS2G();
};

using rfi = NormalFactorIndex;
using rfa = NormalFactorArray;
using rpc = RationalModel::PolyCoeff3rd;
using rpa = RationalModel::PolyCoefArray;
using rcc = RationalModel::CorrCoeff;
using rca = RationalModel::CorrAlgo;
using UPRationalModel = std::unique_ptr<RationalModel>;
using SPRationalModel = std::shared_ptr<RationalModel>;

#include "detail/rationalmodel.hh"
} // namespace earthxpert
