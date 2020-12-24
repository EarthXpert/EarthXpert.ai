
// data types

//structures

// classes

// free functions
m.def("matrixMultiply", [](const pb::EigenDRef<const Eigen::MatrixXd> A, 
  const pb::EigenDRef<const Eigen::MatrixXd> B) -> Eigen::MatrixXd {
    if (A.cols() != B.rows()) 
      throw std::domain_error("Nonconformable matrices!");
    return A * B; }, 
  pb::arg("A"), pb::arg("B"));

m.def("normL2", [](pb::array_t<double> a) -> double {
  auto r = a.unchecked<1>();
  double sumsq = 0;
  for (ssize_t i = 0; i < r.shape(0); i++)
    sumsq += r[i] * r(i); // Either notation works for a 1D array
  return sumsq; });

//m.def("addTwo", [](xt::pyarray<double>& a) -> xt::pyarray<double> {
//  return (a + 2); });
