
/**
 *  \brief Compute correction to radial distortion.
 *
 *  \param [in] sensor the measured position (x, y) where the ray actually
 *  hits the focal plane
 *  \return the radial correction.
 */
inline Point2d FrameIP::CalcRadialCorr(const Point2d& sensor) const {
  if (!mDistFlags[kRADDIST])
    return { 0.0, 0.0 };

  return CalcRadialCorr_Unchecked(sensor);
}

/**
 *  \brief Compute correction to decentering distortion.
 *
 *  \param [in] sensor the measured position (x, y) where the ray actually
 *  hits the focal plane
 *  \return the decentering correction.
 */
inline Point2d FrameIP::CalcDecentCorr(const Point2d& sensor) const {
  if (!mDistFlags[kDECDIST])
    return { 0.0, 0.0 };

  return CalcDecentCorr_Unchecked(sensor);
}

/**
 *  \brief Compute correction to in-plane linear distortion.
 *
 * \param [in] sensor the measured position where the ray actually hits the
 * focal plane
 * \return the linear correction.
 */
inline Point2d FrameIP::CalcLinearCorr(const Point2d& sensor) const {
  if (!mDistFlags[kLINDIST])
    return { 0.0, 0.0 };

  return CalcLinearCorr_Unchecked(sensor);
}

/**
 *  \brief Compute correction to grid distortion.
 *
 * \param [in] sensor the apprant position where the ray would hit the
 * focal plane after correcting radial and decentering distortions.
 * \return the grid correction.
 */
inline Point2d FrameIP::CalcGridCorr(const Point2d& sensor) const {
  if (!mDistFlags[kGRDDIST])
    return { 0.0, 0.0 };

  return CalcGridCorr_Unchecked(sensor);
}

inline Point2d FrameIP::CalcLinearCorr_Unchecked(const Point2d& sensor) const {
  return { mLinearDist[0] * sensor.x + mLinearDist[1] * sensor.y, 0.0 };
}

inline Point2d FrameIP::CalcGridCorr_Unchecked(const Point2d& sensor) const {
  auto [dX, dY] = mGridDist->GetDistortion(float(sensor.x), float(sensor.y));
  return { double(dX), double(dY) };
}
