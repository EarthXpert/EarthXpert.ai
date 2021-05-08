#include <gsl/gsl_poly.h>
#include <cstdio>
#include <memory>

using namespace earthxpert;

TEST(gsl, poly_root) {
  UPGslPolyComplexWorkspace w(gsl_poly_complex_workspace_alloc(6));

  /* coefficients of P(x) =  -1 + x^5  */
  double z[10];
  double a[6] = { -1, 0, 0, 0, 0, 1 };
  gsl_poly_complex_solve(a, 6, w.get(), z);

  for (int i = 0; i < 5; i++) {
    printf ("z%d = %+.18f %+.18f\n", i, z[2*i], z[2*i+1]);
  }

  double b[6] = { 6, 1, 2, 3, 4, 5 };
  gsl_poly_complex_solve(b, 6, w.get(), z);

  for (int i = 0; i < 5; i++) {
    printf ("z%d = %+.18f %+.18f\n", i, z[2*i], z[2*i+1]);
  }
}
