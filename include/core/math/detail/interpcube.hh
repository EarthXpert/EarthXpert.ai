
template <typename Ty>
bool InterpCube<Ty>::operator!=(const InterpCube& rhs) const {
  return !(operator==(rhs));
}

template <typename Ty>
Ty InterpCube<Ty>::GetVal1(Ty x, Ty y, Ty z) const {
  assert(1 <= mDim);
  Ty v;
  GetVal(x, y, z, &v, 1);
  return v;
}

template <typename Ty>
std::pair<Ty, Ty> InterpCube<Ty>::GetVal2(Ty x, Ty y, Ty z) const {
  assert(2 <= mDim);
  Ty v[2];
  GetVal(x, y, z, v, 2);
  return { v[0], v[1] };
}

template <typename Ty>
Tuple3<Ty> InterpCube<Ty>::GetVal3(Ty x, Ty y, Ty z) const {
  assert(3 <= mDim);
  Ty v[3];
  GetVal(x, y, z, v, 3);
  return { v[0], v[1], v[2] };
}
