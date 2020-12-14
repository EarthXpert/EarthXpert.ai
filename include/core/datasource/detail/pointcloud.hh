inline void Pointcloud::SetPointCount(const size_t pointCount) {
  if (pointCount > mPointCapacity)
    throw std::invalid_argument(_("SetPointCount: pointCount"));
  mPointCount = pointCount;
}

inline size_t Pointcloud::GetBufferNumBytes() const noexcept {
  return (mXYZ.size() * sizeof(XYZF) + mCla.size() * sizeof(Classification) +
    mIntensity.size() * sizeof(Intensity) + mRGB.size() * sizeof(RGB16));
}

inline void Pointcloud::SetOrigin(const Point3d& origin) {
  assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
  mOrigin = origin;
}

template <typename FieldTy>
inline void Pointcloud::SetField(std::vector<FieldTy>&& fields) {
  VerifyFieldType<FieldTy>();
  assert(mPointCapacity == fields.size());
  SetField_Unchecked<FieldTy>(std::move(fields));
}

template <typename FieldTy>
inline void Pointcloud::SetField(const size_t i, const FieldTy& field) {
  VerifyFieldType<FieldTy>();
  assert(i < mPointCapacity);
  SetField_Unchecked<FieldTy>(i, field);
}

inline void Pointcloud::SetXYZ(const size_t i, const Point3f xyz) {
  assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
  assert(i < mPointCapacity);
  mXYZ[i].x = xyz.x;
  mXYZ[i].y = xyz.y;
  mXYZ[i].z = xyz.z;
}

inline void Pointcloud::SetXYZ(const size_t i, const XYZD xyz) {
  SetXYZ(i, static_cast<Point3d>(xyz));
}
inline void Pointcloud::SetXYZ(const size_t i, const Point3d xyz) {
  assert(mFieldGroup.test(FieldGroup::kPOSXYZ));
  assert(i < mPointCapacity);
  mXYZ[i].x = xyz.x - mOrigin.x;
  mXYZ[i].y = xyz.x - mOrigin.y;
  mXYZ[i].y = xyz.x - mOrigin.y;
}

template <typename FieldTy>
inline const std::vector<FieldTy>& Pointcloud::GetField() const {
  VerifyFieldType<FieldTy>();
  return GetField_Unchecked<FieldTy>();
}
template <typename FieldTy> 
inline std::vector<FieldTy>& Pointcloud::GetField() {
  VerifyFieldType<FieldTy>();
  return GetField_Unchecked<FieldTy>();
}

inline XYZD Pointcloud::GetXYZD(const size_t i) noexcept {
  return XYZD{ mOrigin.x + mXYZ[i].x,
    mOrigin.y + mXYZ[i].y, mOrigin.z + mXYZ[i].z };
}
inline Point3d Pointcloud::GetPoint3d(const size_t i) noexcept {
  return static_cast<Point3d>(GetXYZD(i));
}

template <typename FieldTy>
inline const FieldTy& Pointcloud::GetField(const size_t i) const {
  VerifyFieldType<FieldTy>();
  if (mPointCapacity <= i)
    throw std::out_of_range(_("Pointcloud::GetField()"));

  return GetField_Unchecked<FieldTy>(i);
}

template <typename FieldTy>
inline FieldTy& Pointcloud::GetField(const size_t i) {
  VerifyFieldType<FieldTy>();
  if (mPointCapacity <= i)
    throw std::out_of_range(_("Pointcloud::GetField()"));

  return GetField_Unchecked<FieldTy>(i);
}

template <typename PointTy>
inline void Pointcloud::SetPoint(const size_t i, const PointTy& point) {
  if (mPointCapacity <= i)
    throw std::out_of_range(_("Pointcloud::SetPoint()"));
  VerifyPointFieldBits(PointTy::kPointFieldBits);

  SetPoint_Unchecked<PointTy>(i, point);
}

inline void Pointcloud::SetPoint(const size_t i, const LASpoint& point,
  const LASquantizer* quantizer) {
  if (mPointCapacity <= i)
    throw std::out_of_range(_("Pointcloud::SetPoint()"));

  SetPoint_Unchecked(i, point, quantizer);
}

template <typename PointTy>
inline PointTy Pointcloud::GetPoint(const size_t i) const {
  if (mPointCapacity <= i)
    throw std::out_of_range(_("Pointcloud::GetPoint()"));
  VerifyPointFieldBits(PointTy::kPointFieldBits);

  return GetPoint_Unchecked<PointTy>(i);
}

template <typename FieldTy>
inline void Pointcloud::SetField_Unchecked(std::vector<FieldTy>&& fields) {
  if constexpr (IsXYZF<FieldTy>) {
    mXYZ = std::move(fields);
  }
  if constexpr (IsClassification<FieldTy>) {
    mCla = std::move(fields);
  }
  if constexpr (IsIntensity<FieldTy>) {
    mIntensity = std::move(fields);
  }
  if constexpr (IsRGB16<FieldTy>) {
    mRGB = std::move(fields);
  }
  // \todo add other field groups
}

template <typename FieldTy>
inline const std::vector<FieldTy>& Pointcloud::GetField_Unchecked() const {
  if constexpr (IsXYZF<FieldTy>) {
    return mXYZ;
  }
  if constexpr (IsClassification<FieldTy>) {
    return mCla;
  }
  if constexpr (IsIntensity<FieldTy>) {
    return mIntensity;
  }
  if constexpr (IsRGB16<FieldTy>) {
    return mRGB;
  }
  // \todo add other field groups
  assert(false && "unknown filed type.");
}

template <typename FieldTy>
inline std::vector<FieldTy>& Pointcloud::GetField_Unchecked() {
  if constexpr (IsXYZF<FieldTy>) {
    return mXYZ;
  }
  if constexpr (IsClassification<FieldTy>) {
    return mCla;
  }
  if constexpr (IsIntensity<FieldTy>) {
    return mIntensity;
  }
  if constexpr (IsRGB16<FieldTy>) {
    return mRGB;
  }
  // \todo add other field groups
  assert(false && "wrong field type.");
}

template <typename FieldTy>
inline void Pointcloud::SetField_Unchecked(const size_t i, const FieldTy& field) {
  if constexpr (IsXYZF<FieldTy>) {
    mXYZ[i] = field;
  }
  if constexpr (IsClassification<FieldTy>) {
    mCla[i] = field;
  }
  if constexpr (IsIntensity<FieldTy>) {
    mIntensity[i] = field;
  }
  if constexpr (IsRGB16<FieldTy>) {
    mRGB[i] = field;
  }
  // \todo add other field groups
}

template <typename FieldTy>
inline const FieldTy& Pointcloud::GetField_Unchecked(const size_t i) const {
  if constexpr (IsXYZF<FieldTy>) {
    return mXYZ[i];
  }
  if constexpr (IsClassification<FieldTy>) {
    return mCla[i];
  }
  if constexpr (IsIntensity<FieldTy>) {
    return mIntensity[i];
  }
  if constexpr (IsRGB16<FieldTy>) {
    return mRGB[i];
  }
  // \todo add other field groups
  assert(false && "wrong field type.");
}

template <typename FieldTy>
inline FieldTy& Pointcloud::GetField_Unchecked(const size_t i) {
  if constexpr (IsXYZF<FieldTy>) {
    return mXYZ[i];
  }
  if constexpr (IsClassification<FieldTy>) {
    return mCla[i];
  }
  if constexpr (IsIntensity<FieldTy>) {
    return mIntensity[i];
  }
  if constexpr (IsRGB16<FieldTy>) {
    return mRGB[i];
  }
  // \todo add other field groups
  assert(false && "wrong field type.");
}

template <typename PointTy>
inline void Pointcloud::SetPoint_Unchecked(const size_t i, const PointTy& point) {
  if constexpr (std::is_base_of_v<XYZF, PointTy>) {
    mXYZ[i] = static_cast<const XYZF&>(point);  // SetField<XYZF>()
  }
  if constexpr (std::is_base_of_v<Classification, PointTy>) {
    mCla[i].c = point.c;
  }
  if constexpr (std::is_base_of_v<Intensity, PointTy>) {
    mIntensity[i].i = point.i;
  }
  if constexpr (std::is_base_of_v<RGB16, PointTy>) {
    mRGB[i] = static_cast<const RGB16&>(point);
  }
  // \todo add other field groups
}

template <typename PointTy>
inline PointTy Pointcloud::GetPoint_Unchecked(const size_t i) const {
  PointTy point;
  if constexpr (std::is_base_of_v<XYZF, PointTy>) {
    auto& xyz = static_cast<XYZF&>(point);
    xyz = mXYZ[i];    // GetField<XYZF>(i)
  }
  if constexpr (std::is_base_of_v<Classification, PointTy>) {
    point.c = mCla[i].c;
  }
  if constexpr (std::is_base_of_v<Intensity, PointTy>) {
    point.i = mIntensity[i].i;
  }
  if constexpr (std::is_base_of_v<RGB16, PointTy>) {
    auto& rgb = static_cast<RGB16&>(point);
    rgb = mRGB[i];
  }
  // \todo add other field groups

  return point;
}

inline void Pointcloud::VerifyPointFieldBits(const size_t pointFieldBits) const {
  bool good{ true };
  for (auto [pos, bit] : FieldGroup::kPOSBIT) {
    if (bit & pointFieldBits) {
      good = good && mFieldGroup.test(pos);
    }
  }

  if (!good)
    throw std::invalid_argument(_("VerifyPointFieldBits: pointFieldBits"));
}

template <typename FieldTy> 
inline void Pointcloud::VerifyFieldType() const {
  static_assert(IsField_v<FieldTy>, "VerifyFieldType: FieldTy");
  if (!mFieldGroup.test(FieldTy::kFieldPos))
    throw std::invalid_argument(_("VerifyFieldType: FieldTy"));
}
