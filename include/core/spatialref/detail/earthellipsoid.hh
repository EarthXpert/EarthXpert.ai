
/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
inline bool EarthEllipsoid::operator!=(const EarthEllipsoid& rhs) const {
  return !(operator==(rhs));
}

inline std::unique_ptr<EarthEllipsoid> EarthEllipsoid::Clone() const {
  return std::make_unique<EarthEllipsoid>(*this);
}

inline double EarthEllipsoid::GetSemiMajor() const noexcept { return mA; }
inline double EarthEllipsoid::GetSemiMinor() const noexcept { return mB; }
inline double EarthEllipsoid::GetEccSquared() const noexcept { return mEccSq; }

inline Point3d EarthEllipsoid::LPHToXYZ(const Point3d& lph) const {
  return LPHToXYZ(lph.x, lph.y, lph.z);
}

inline Point3d EarthEllipsoid::XYZToLPH(const Point3d& xyz) const {
  return XYZToLPH(xyz.x, xyz.y, xyz.z);
}

inline Point3d EarthEllipsoid::LSRToXYZ(const Point3d& lsr) const {
  const cv::Mat E = mL2E * cv::Mat(lsr);
  const Point3d lsrE(E.at<double>(0), E.at<double>(1), E.at<double>(2));
  return (mLsrOriginXYZ + lsrE);
}
inline Point3d EarthEllipsoid::LSRToXYZ(double X, double Y, double Z) const {
  return LSRToXYZ(Point3d(X, Y, Z));
}

inline Point3d EarthEllipsoid::XYZToLSR(const Point3d& xyz) const {
  const cv::Mat L = mL2E.t() * cv::Mat(xyz - mLsrOriginXYZ);
  return { L.at<double>(0), L.at<double>(1), L.at<double>(2) };
}
inline Point3d EarthEllipsoid::XYZToLSR(double X, double Y, double Z) const {
  return XYZToLSR(Point3d(X, Y, Z));
}

inline Point3d EarthEllipsoid::LSRToLPH(const Point3d& lsr) const {
  const Point3d xyz = LSRToXYZ(lsr);
  return XYZToLPH(xyz);
}

inline Point3d EarthEllipsoid::LSRToLPH(double X, double Y, double Z) const {
  return LSRToLPH(Point3d(X, Y, Z));
}

inline Point3d EarthEllipsoid::LPHToLSR(const Point3d& lph) const { 
  return XYZToLSR(LPHToXYZ(lph)); 
}

inline Point3d EarthEllipsoid::LPHToLSR(double lon, double lat, double height) const {
  return LPHToLSR(Point3d(lon, lat, height));
}

inline double EarthEllipsoid::RadiusInPrimeMeridian(const double lat) const {
  const double sp = sin(lat);
  return (mA * mBsq_Asq) / pow(1.0 - mEccSq * sp * sp, 1.5);
}

inline double EarthEllipsoid::RadiusInPrimeVertical(const double cp) const {
  const double cp2 = cp * cp;
  return mAsq / sqrt(mAsq * cp2 + mBsq * (1.0 - cp2));
}
