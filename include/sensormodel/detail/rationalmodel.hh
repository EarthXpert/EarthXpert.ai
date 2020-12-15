/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */

 /**
  *  Test inequality.
  *
  *  \param [in] rhs Object to compare with
  *  \return true if not equal.
  */
inline bool RationalModel::operator!=(const RationalModel& rhs) const {
  return !(operator==(rhs));
}

inline Point2d RationalModel::ImageApparantToActual(const Point2d& image) const {
  Point2d imageAct{ image };
  if (mCorrFlag) {  // apply correction
    const auto imageCorr = CalcCorrection(image);
    imageAct += imageCorr;
  }
  return imageAct;
}

inline Point2d RationalModel::ImageApparantToSensor(const Point2d& image) const {
  return { (image.x - nfc[eOSAMP]) * mInvScale[eISSAMP],
           (image.y - nfc[eOLINE]) * mInvScale[eISLINE] };
}

inline Point2d RationalModel::ImageToSensor(const Point2d& image) const {
  const Point2d imageAct{ image.x - 0.5, image.y - 0.5 };
  const auto imageApp = ImageActualToApparant(imageAct);
  return ImageApparantToSensor(imageApp);
}

inline Point2d RationalModel::SensorToImageApparant(const Point2d& sensor) const {
  return { nfc[eOSAMP] + nfc[eSSAMP] * sensor.x,
           nfc[eOLINE] + nfc[eSLINE] * sensor.y };
}

inline Point2d RationalModel::SensorToImageActual(const Point2d& sensor) const {
  const auto imageApp = SensorToImageApparant(sensor);
  return ImageApparantToActual(imageApp);
}

inline Point3d RationalModel::GroundToVirtual(const Point3d& ground) const {
  return { (ground.x - nfc[eOLON]) * mInvScale[eISLON],
           (ground.y - nfc[eOLAT]) * mInvScale[eISLAT],
           (ground.z - nfc[eOHGT]) * mInvScale[eISHGT] };
}

inline bool RationalModel::VirtualToSensor(const Point3d& uvw, Point2d& sensor) const {
  const auto [Ln, Ld, Sn, Sd] = CalcPolynomials(uvw);

  // col/row computed from RFM
  sensor.x = Sn / Sd;
  sensor.y = Ln / Ld;
  return true;
}
