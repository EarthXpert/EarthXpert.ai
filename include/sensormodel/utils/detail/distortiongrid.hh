
/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return true if not equal.
 */
template <typename Ty>
bool DistortionGrid<Ty>::operator!=(const DistortionGrid<Ty>& rhs) const {
  return !(operator==(rhs));
}

template <typename Ty>
bool DistortionGrid<Ty>::GetDistortion(const Ty x, const Ty y, Ty* const d) const {
  if (mGrid) {
    assert(d);
    mGrid->GetVal(x, y, d);
    return true;
  }
  return false;
}

template <typename Ty>
std::pair<Ty, Ty> DistortionGrid<Ty>::GetDistortion(const Ty x, const Ty y) const {
  if (mGrid) {
    assert(2 == mGrid->GetDimension());
    return mGrid->GetVal2(x, y);
  }
  return { 0.f, 0.f };
}
