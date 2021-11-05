
/**
 * Test inequality.
 *
 * \param [in] rhs Object to compare with
 * \return Returns true if not equal.
 */
template <typename Ty>
bool InterpGrid<Ty>::operator!=(const InterpGrid<Ty>& rhs) const {
  return !(operator==(rhs));
}

template <typename Ty>
Ty InterpGrid<Ty>::GetVal1(Ty X, Ty Y) const {
  assert(1 <= mDim);
  Ty val;
  GetVal(X, Y, &val, 1);
  return val;
}

template <typename Ty>
std::pair<Ty, Ty> InterpGrid<Ty>::GetVal2(Ty X, Ty Y) const {
  assert(2 <= mDim);
  Ty val[2];
  GetVal(X, Y, val, 2);
  return { val[0], val[1] };
}

template <typename Ty>
Tuple3<Ty> InterpGrid<Ty>::GetVal3(Ty X, Ty Y) const {
  assert(3 <= mDim);
  Ty val[3];
  GetVal(X, Y, val, 3);
  return { val[0], val[1], val[2] };
}
