inline bool StereoModel::ImagesToGround(const Point2d &leftImage,
  const Point2d &rightImage, Point3d &ground) const {
  assert(2 == mModels.size());
  switch (mFwdAlgo) {
  case eFA_INTERSECT: {
    Point2d images[2] = { leftImage, rightImage };
    return ImagesToGroundX(mModels, images, 2, ground);
  }
  case eFA_CERES: {
    return ImagesToGroundCeres(leftImage, rightImage, ground);
  }
  case eFA_LS: {
    return ImagesToGroundLS(leftImage, rightImage, ground);
  }
  case eFA_NR: {
    return ImagesToGroundNR(leftImage, rightImage, ground);
  }
  default: {
    return false;
  }
  }
}

inline bool StereoModel::ImagesToGround(const std::array<Point2d, 3> &images,
  Point3d &ground) const {
  assert(3 == mModels.size());
  switch (mFwdAlgo) {
  case eFA_INTERSECT: {
    return ImagesToGroundX(mModels, images.data(), 3, ground);
  }
  case eFA_CERES: {
    return ImagesToGroundCeres(images, ground);
  }
  case eFA_LS: {
    return ImagesToGroundLS(images, ground);
  }
  case eFA_NR: {
    return ImagesToGroundNR(images, ground);
  }
  default: {
    return false;
  }
  }
}

inline bool StereoModel::ImagesToGround(const std::vector<Point2d> &images,
  Point3d &ground) const {
  return ImagesToGround(mModels, images, ground);
}

inline bool StereoModel::ImagesToGround(const std::vector<const ISensorModel*>& sensorModels,
  const std::vector<Point2d>& images, Point3d& ground) const {
  const int numRays = int(sensorModels.size());
  assert(numRays == images.size());

  switch (mFwdAlgo) {
  case eFA_INTERSECT: {
    return ImagesToGroundX(sensorModels, images.data(), numRays, ground);
  }
  case eFA_CERES: {
    return ImagesToGroundCeres(sensorModels, images.data(), numRays, ground);
  }
  case eFA_LS: {
    //return ImagesToGroundLS(sensorModels, images.data(), numRays, ground);
    throw std::exception(_("StereoModel: not implemented"));
  }
  case eFA_NR: {
    //return ImagesToGroundNR(sensorModels, images.data(), numRays, ground);
    throw std::exception(_("StereoModel: not implemented"));
  }
  default: {
    return false;
  }
  }
}

inline double StereoModel::GetMeanHeight(
  const std::vector<const ISensorModel*>& models) const noexcept {
  assert(0 < models.size());
  double z{};
  for (auto model : models) {
    z += model->GetMeanHeight();
  }

  return (z / double(models.size()));
}
